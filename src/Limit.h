#ifndef LIMIT_H
#define LIMIT_H

#include "Signal.h"
#include <math.h>
/** Abstract class to represent any kind of limit.
* The basic idea is that an exception is thrown when a limit is reached
*/
class Limit {
protected:
	//! The value for an upper limit
	double _posLimit;
	//! The value for a lower limit
	double _negLimit;
	//! One can en/disable the upper limits
	bool _isPosLimitEnabled;
	//! One can en/disable the lower limits
	bool _isNegLimitEnabled;
public:
	//! Simple constructor
	Limit() : _posLimit(0), _negLimit(0), _isPosLimitEnabled(false), _isNegLimitEnabled(false) {};
	//! Active checking of limits
	virtual void checkLimit() = 0;
	/** Enables the upper limit
	* @param b set to false to disable
	*/
	void enablePosLimit(bool b = true) {_isPosLimitEnabled = b;}
	
	/** Enables the lower limit
	* @param b set to false to disable
	*/
	void enableNegLimit(bool b = true) {_isNegLimitEnabled = b;}
	
	/** Setter for the absulute value of upper limit
	* @param limit
	*/
	void setPosLimit (double l);
	
	/** Setter for the absulute value of lower limit
	* @param limit
	*/
	void setNegLimit (double l);
	
	//!Getter for the upper limit
	double getPosLimit() { return _posLimit;}
	
	//!Getter for the lower limit
	double getNegLimit() { return _negLimit;}
	
	bool isNegLimitEnabled() { return _isNegLimitEnabled; }
	
	bool isPosLimitEnabled() { return _isPosLimitEnabled; }
	
};

#ifdef __CINT__
#pragma link C++ class Limit-;
#endif

#endif