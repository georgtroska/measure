#include "Scan1D.h"

#include <TMath.h>
#include <cmath>

using namespace std;

MeasEvent* Scan1D::createBORE() {
	MeasEvent * bore = new MeasEvent(true);
	int nChannels = _channels.size();
	for (int n = 0; n < nChannels; n++) {
		ChannelMeasEvent chEvent;
		chEvent.value = -1;
		chEvent.RMS = -1;
		chEvent.singleValue.push_back(-1);
		bore->channelEvent.push_back(chEvent);
	}
	bore->param = -1;
	return bore;
}

vector<MeasureChannel*> Scan1D::map2vector (map<string, MeasureChannel> m) {
	vector<MeasureChannel*> v;
	map<string, MeasureChannel>::iterator it;
	for( it = m.begin(); it != m.end(); it++) {
		v.push_back(&(it->second));
	}
	return v;
}

Scan1D::Scan1D(Param * v) : _param(v), _rep(true){
	_param->getDevice().getSignal().Connect("changedParam()", "Scan1D", this, "checkLimits()");
}

void Scan1D::Run(double start, double stop,  double step, double sleepSet){
	vector<double> steps;
	if (stop < start) step = -fabs(step);
	bool isLooping = true;
	for (double u = start; isLooping; u += step){
		(start < stop) ? isLooping = (u < stop) : isLooping = (u > stop);
		steps.push_back(u);
	}
	Run(steps, sleepSet);
}

 void Scan1D::RunLog(double start, double stop, int nPoints, double sleepSet) {
	for (int i = 0; i < _graphs.size(); i++){
		_graphs.at(i)->applyLog();
	}
	vector<double> steps;
	bool isLooping = true;
	for (int n = 0; n < nPoints; n++){
		double u = start*exp(n*std::log(stop/start)/(nPoints-1));
		steps.push_back(u);
	}
	Run(steps, sleepSet);
}

void Scan1D::RunLogDecades(double start, double stop, int stepsPerDecade, double sleepSet) {
	for (int i = 0; i < _graphs.size(); i++){
		_graphs.at(i)->applyLog();
	}
	
	int decade = std::floor(abs(log10(start))); //with start = 22, decade -> 1
	double freq = start/TMath::Power(10,decade); //with start = 22, freq -> 2.2
	double myFreq;
	for (int myStep = 0; myStep < stepsPerDecade; myStep++) {
		myFreq = TMath::Power(TMath::Power(10,1./stepsPerDecade),myStep);
		if (myFreq > freq) break;
	}
	
	vector<double> steps;
	//Pushing in the startFreq
	steps.push_back(TMath::Power(10,decade)*freq);
	bool isLooping = true;
	while (isLooping) {
		for (int myStep = 0; myStep < stepsPerDecade; myStep++) {
			myFreq = TMath::Power(TMath::Power(10,1./stepsPerDecade),myStep);

			if (TMath::Power(10,decade)*myFreq > stop) {
				isLooping = false;
				break;
			}
			if (TMath::Power(10,decade)*myFreq >= freq*TMath::Power(10,decade)) steps.push_back(TMath::Power(10,decade)*myFreq);
		}
		decade++; 
	}
	if (TMath::Power(10,decade)*myFreq > stop) {
		steps.push_back(stop);
	}
	Run(steps, sleepSet);
}

void Scan1D::Run(vector<double>& steps, double sleepSet) {
	try{
		MeasEvent * bore = createBORE();
		onStartRun(bore);
		delete bore;
		std::cout << "Scan1D: Starting new measurement..." << std::endl;
		vector<double>::iterator stepit;
		bool isFirst = true;
		for (stepit = steps.begin(); stepit < steps.end(); stepit++){
			(*_param)(*stepit);
			if (isFirst) sleep(1); else sleep(sleepSet);		
			MeasEvent * event = new MeasEvent();
			event->param = *stepit;
			map<string,MeasureChannel>::iterator it;
			for( it = _channels.begin(); it != _channels.end(); it++){
				ChannelMeasEvent chEvent;
				chEvent.singleValue = it->second.Run();	//Running Measurement here
				chEvent.value = it->second.getMean();
				chEvent.RMS = it->second.getRMS();
				event->channelEvent.push_back(chEvent);
				//std::cout << "Mean: " << ch.getMean() << "; RMS: " << ch.getRMS() << std::endl;
			}
			onData(event);	//sending measurement-data via signal
			delete event;
			isFirst = false;
		}
		if(_rep) _param->initialize();
	}
	catch(ExcLimit &e) {
		std::cerr << "EXCEPTION: Limit reached in  " << e.getLimitKind() << std::endl;
		std::cerr << "Device: " << e.getDevName() << "; Limit: " << e.getLimitVal() << "; Last value: " << e.getLastVal() << std::endl;
		std::cerr << "Parameter will be re-initialized..." << std::endl;
		double lastParam = (*_param)();
		_param->initialize();
		std:cerr << "Saving last value..." << std::endl;
		MeasEvent * lastEvent = new MeasEvent();
		lastEvent->param = lastParam;
		
		ChannelMeasEvent lastChEvent;
		lastChEvent.singleValue.push_back(e.getLastVal());
		lastChEvent.value = e.getLastVal();
		lastChEvent.RMS = 0;
		
		ChannelMeasEvent emptyChEvent;
		emptyChEvent.singleValue.push_back(0);
		emptyChEvent.value = 0;
		emptyChEvent.RMS = 0;
		
		map<string,MeasureChannel>::iterator it;
		for (it = _channels.begin(); it != _channels.end(); it++) {
			if (it->first.compare(e.getChannelName())) /* compare returns 0 if strings are equal*/ lastEvent->channelEvent.push_back(emptyChEvent);
			else lastEvent->channelEvent.push_back(lastChEvent);
 		}
		onData(lastEvent);
		delete lastEvent;
		
		
	}
	catch(std::exception & e){
		std::cerr << e.what() << std::endl; 
		std::cerr << "Parameter will be re-initialized..." << std::endl;
		_param->initialize();
	}
	catch(...) {
		std::cerr << "Unspecified Exception raised in Scan1D.Run()-Function." << std::endl;
		std::cerr << "Parameter will be re-initialized..." << std::endl;
		_param->initialize();
	}
}


vector<Channel*> Scan1D::getChannels() { 
	vector<Channel*> v;
	map<string, MeasureChannel>::iterator it;
	for( it = _channels.begin(); it != _channels.end(); it++) {
		v.push_back(it->second.getChannel());
	}
	return v;
}

void Scan1D::checkLimits () {
	//std::cout << "Called checkLimits() by Signal" << std::endl;
	_param->checkLimit();
	map<string,MeasureChannel>::iterator it;
	for( it = _channels.begin(); it != _channels.end(); it++){
		it->second.getChannel()->checkLimit();
	}
}

void Scan1D::drawOnline(){
	int size = _channels.size();
	Window1DGraph* tempGraph;
	for(int i=0; i<size ; i++){
		tempGraph = new Window1DGraph(this, 400 ,600, (400*i), 0, i);
		_graphs.push_back(tempGraph);
		Connect("onData(MeasEvent*)", "Window1DGraph", _graphs.back(), "addPoint(MeasEvent*)");
		
	}
	/*_graph = new Window1DGraph(this, 400, 600,0,0,0);
	Connect("onData(MeasEvent*)", "Window1DGraph", _graph, "addPoint(MeasEvent*)");
	_graphTemp = new Window1DGraph(this, 400, 600,400,0,1);
	Connect("onData(MeasEvent*)", "Window1DGraph", _graphTemp, "addPoint(MeasEvent*)");*/
}

Scan1D::~Scan1D() {
	for (int i = 0; i < _graphs.size(); i++){
		delete _graphs.at(i);
	}
	
}

void Scan1D::reinitializeParamAfterScan(bool rep) {
	_rep = rep;
}
