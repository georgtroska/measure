#ifndef SIGNAL_H
#define SIGNAL_H

#include <RQ_OBJECT.h>


class __declspec(dllexport) Signal {
	RQ_OBJECT("Signal")
	
public:
	void changedParam();						//*SIGNAL* 
	void measuredOnce(double val);				//*SIGNAL*
	void measuredMean(double mean, double rms);	//*SIGNAL*
};

#ifdef __CLING__
#pragma link C++ class Signal-;
#endif

#endif