#ifndef CHANNEL_H
#define CHANNEL_H

#include "Limit.h"
#include "Device.h"
#include <string>
/** Abstract class that represents a measurement channel in a device. E.g. the current-channel in an amperemeter
*/
class Channel : public Limit {
protected:
	//! Simple kind of this channel
	std::string _kind;
	//! The savename for this channel in the saved file
	std::string _saveName;
	//! The unit for this channel. e.g. A, oder V (only standard/base units)
	std::string _unit;
public:
	//! Constructor - empty
	Channel() : Limit() {};
	/** Abstract reader for values
	* @return the value of the attached device
	*/
	virtual double operator()() = 0;
	
	//! Abstract getter for the attached device
	virtual Device & getDevice() = 0;
	
	//Limit functions
	/**Active checking of the limits. Throws an exception if limit is reached
	*/
	virtual void checkLimit();
	//end Limit functions
	
	//! Setter for the save name
	void setSaveName (std::string name) {_saveName = name;}
	//! Getter for the save name
	std::string getSaveName () {return _saveName; }
	//! Setter for the Kind
	void setKind(std::string kind) { _kind = kind;}
	//! Getter for the Kind
	std::string getKind() { return _kind; }
	//! Setter for the Unit
	void setUnit(std::string unit) { _unit = unit;}
	//! Getter for the Unit
	std::string getUnit() { return _unit; }
};

#ifdef __CINT__
#pragma link C++ class Channel-;
#endif

#endif