#ifndef PARAM_H
#define PARAM_H

#include "Device.h"
#include "Channel.h"
#include <math.h>
/** Abstract class that represents a manipulable parameter in a device. E.g. the voltage-parameter in a power supply
*/
class __declspec(dllexport) Param : public Channel  {
public:
	//! Constructor empty
	Param(): Channel() {
		_isPosLimitEnabled = true;
		_posLimit = 0;
	}
	/** Abstract reader for this parameter
	* @return the value of the parameter
	*/
	virtual double operator()() = 0;
	
	/** Abstract writer for this parameter
	* @param the value the parameter should be set to
	*/
	virtual void operator()(double v) = 0;
	//! Brings the parameter in its initial state
	virtual void initialize () = 0;
	//! Abstract getter for the attached device - Necessary for the Limit class
	virtual Device & getDevice() = 0;
};

#ifdef __CLING__
#pragma link C++ class Param-;
#endif

#endif