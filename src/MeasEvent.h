#ifndef MEASEVENT_H
#define MEASEVENT_H

#include <vector>
#include <stdint.h>

class __declspec(dllexport) ChannelMeasEvent {
public:
	double value;
	double RMS;
	std::vector<double> singleValue;
	ChannelMeasEvent() {};	
	/*ChannelMeasEvent(const ChannelMeasEvent &a) {
		value = a.value;
		RMS = a.RMS;
		singleValue = a.singleValue;
	}*/
};

class __declspec(dllexport) MeasEvent {
public:
	bool isBORE;
	double param;
	std::vector<ChannelMeasEvent> channelEvent;
	double time;
	MeasEvent( bool BORE = false ) : isBORE(BORE) {
		//time = dce::DCESystem::getRealTime() / 1e9;
		time = 0; //FIXME
		
	}
	
	/*MeasEvent(const MeasEvent &a) {
		param = a.param;
		channelEvent = a.channelEvent;
		time = a.time;
	}*/
	
	MeasEvent & operator=(const MeasEvent &a ) {
		
		param = a.param;
		time = a.time;
		channelEvent = a.channelEvent;
		
		return *this;
	}
	
};

#ifdef __CLING__
#pragma link C++ class MeasEvent-;
#pragma link C++ class ChannelMeasEvent-;
#endif



#endif
