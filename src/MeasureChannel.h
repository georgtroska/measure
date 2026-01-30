#ifndef MEASURECHANNEL_H
#define MEASURECHANNEL_H

#include "Channel.h"
#include <vector>


class __declspec(dllexport) MeasureChannel {
	Channel* _channel;
	int _nValues;
	std::vector<double> _value;
	double _sleepTime;
public:
	MeasureChannel(): _channel(NULL), _nValues(1), _sleepTime(0){}
	MeasureChannel(Channel* c, int n = 1, double t = 0): _channel(c), _nValues(n), _sleepTime(t) {}
	double getMean();
	double getRMS();
	std::vector<double> Run();
	void initializeTriggeredReadout();
	std::vector<double> readMeasurements();
	void setNValues (int n);
	void setSleepTime (double t);
	Channel* getChannel() { return _channel; } 
	
};

#ifdef __CLING__
#pragma link C++ class MeasureChannel-;
#endif

#endif