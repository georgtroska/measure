#ifndef SCAN1D_H
#define SCAN1D_H

#include "Channel.h"
#include "Param.h"
#include "ExcLimit.h"
#include "MeasureChannel.h"
#include "Signal.h"
#include "MeasEvent.h"
#include "Window1DGraph.h"
#include <iostream> 
#include <vector>
#include <map>
#include <TGClient.h>
class Window1DGraph;
/** This is the implementation for measurements with one changeable parameter and one or more measured channels
* @code
* // create measurement-class with name "scan" with changeable parameter voltage from k487, both graphs for channels 1&2 turned on
* Scan1D *scan = new Scan1D(&k487->voltage);
* //add current from k487 as channel to measure to the measurement with 5 measures per param-value and 0.1s sleeptime between each measurement
* scan->addChannel(&k487->current, 5, 0.1);
* //start measurement from 0 to -300 in previously attached parameter in steps of 5 with sleeptime of 2s after each param-set 
* scan->Run(0,-300,5,2);
* @endcode
*/
class Scan1D {
protected:
	RQ_OBJECT("Scan1D")
	Param * _param;
	std::map<std::string,MeasureChannel> _channels;
	std::vector<Window1DGraph*> _graphs;

	MeasEvent* createBORE();
	std::vector<MeasureChannel*> map2vector (std::map<std::string, MeasureChannel> m);

	bool _rep;

public:
	
	//constructor
	/**
	* Constructor for the class 
	* @param v is a pointer to the changeable parameter  
	*/
	Scan1D(Param * v);
	//constructor end
	
	/**
	* function to start a measurement
	* @param start is the value where to start the measurement
	* @param stop is the value where to stop the measurement
	* @param step are widths in which the parameter is changed
	* @param sleepSet is the time the program waits after the parameter is changed
	*/
	virtual void Run(double start, double stop,  double step, double sleepSet);
	
	virtual void RunLog(double start, double stop, int nPoints, double sleepSet);
	
	virtual void RunLogDecades(double start, double stop, int stepsPerDecade, double sleepSet);

	/**
	* function to start a measurement
	* @param steps is a pointer to a vector which contains all measurement points for the parameter
	* @param sleepSet is the time the program waits after the parameter is changed
	*/
	virtual void Run(std::vector<double>& steps, double sleepSet);
	/**
	* 
	*/
	virtual void checkLimits ();		//*RECEIVER*
	
	/**
	* adds a new channel to the measurement which is measured after each step from the parameter
	* @param c is a pointer to the new channel
	* @param n is the number of single measurements of the new channel per parameter-step
	* @param t is the time to wait between single channel-measurments
	*/
	virtual void addChannel( Channel * c, int n=1, int t=0) { 
		std::string name = c->getSaveName();
		if (_channels.find(name) != _channels.end())	{
			std::cerr << "Please change saveName for Channel " << name << "from Device " << c->getDevice().getName() << std::endl;
			std::cerr << "(if there are more than one channel with that name you only have to change one of those names)" << std::endl;
			std::cerr << "add something like that to your script: (device_name)->(channel_name).setSaveName(\"(name)\");" << std::endl;
			throw;
		}
		_channels[name] = MeasureChannel(c,n,t); 
	}
	
	/**
	* 
	*/
	virtual Channel* getChannel(const std::string s) { 
		if(_channels.find(s) != _channels.end()) return _channels[s].getChannel(); 
		else {
			std::cerr << "Tried to find channel with saveName that doesn't exist." << std::endl;
			throw;
		}
	}
	
	virtual std::vector<Channel*> getChannels();
	
	virtual unsigned int getNChannels() { return _channels.size(); }
	
	virtual Param* getParam() { return _param; }
	
	virtual void drawOnline();
	
	virtual void onData (MeasEvent* event) {				//*SIGNAL*
		Emit("onData(MeasEvent*)", (size_t)event);
	}
	
	virtual void onStartRun (MeasEvent * bore) {			//*SIGNAL*
		Emit("onStartRun(MeasEvent*)", (size_t)bore);
	}
	
	virtual void onEndRun () {			//*SIGNAL*
		Emit("onEndRun()");
	}
	
	virtual void reinitializeParamAfterScan(bool rep);
	
	virtual ~Scan1D();
};

#ifdef __CINT__
#pragma link C++ class Scan1D-;
#endif

#endif
