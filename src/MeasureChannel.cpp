#include "MeasureChannel.h"
#include <math.h>

using namespace std;

double MeasureChannel::getMean() {
	double c_all = 0;
	for (int i=0; i< _nValues; i++)
	{
		c_all += _value.at(i);
	}
	return c_all/_nValues;
}

//!Bestimmung der Standardabweichung einer Messreihe
double MeasureChannel::getRMS(){
	double c_mean = getMean();
	double c_err = 0;
	for (int i = 0; i < _nValues; i++)
	{
		c_err += (_value.at(i)-c_mean)*(_value.at(i)-c_mean);
	}
	c_err /=_nValues;
	return sqrt(c_err);
}

 vector<double> MeasureChannel::Run() {
	_value.clear();
	_value.reserve(_nValues);
	for( int n=0; n<_nValues; n++){
		double newValue = (*_channel)();
		cout << "Measured value: " << newValue << endl;
		_value.push_back(newValue);
		_channel->getDevice().getSignal().measuredOnce(newValue);
		if (_sleepTime != 0) {
			#if defined(__linux__) || defined(__FreeBSD__)
				sleep(_sleepTime);
			#else
				Sleep(_sleepTime);
			#endif		
		}
	}
	_channel->getDevice().getSignal().measuredMean(getMean(), getRMS());
	return _value;
}

void MeasureChannel::initializeTriggeredReadout() {
	_channel->getDevice().setSpecialTriggerMode(EXTERN, ALL);
}

 vector<double> MeasureChannel::readMeasurements() {
	_value.clear();
	_value.reserve(_nValues);

	 _value  = _channel->getDevice().readTriggeredDataAtOnce();

	return _value;
}

void MeasureChannel::setNValues (int n) {
	if(n>0) 	_nValues = n;
	else {
		cerr << "WARNING: Wrong number of single measurements for " << _channel->getKind() << endl;
		cerr << "(value should be > 0) " << endl;
	}
}

void MeasureChannel::setSleepTime (double t) {
	if(t>=0)	 _sleepTime = t; 
	else {
		cerr << "WARNING: Wrong value for sleeptime between measurements for " << _channel->getKind() << endl;
		cerr << "(value should be >= 0) " << endl;
	}	
}