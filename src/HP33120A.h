#ifndef HP33120A_H
#define HP33120A_H

#include "Device.h"
#include "Param.h"

class HP33120A : public Device {
public:
/*
	class Frequency : public Param {
		
	} freq;
	
	class Amplitude : public Param {
		
	} ampl;
	
	class Offset : public Param {
		
	} offset;
	*/
	enum Type {
		kSinusoid,
		kSquare,
		kTriangle,
		kRamp,
		kNoise,
		kDC,
		kUser,
		kUnknownType
	} _type;
	enum VoltageUnit {
		kVpp, //point to point
		kVRMS, //rms
		kVDBm, //DBm
		kUnknownUnit
	} _vunit;

public:
	HP33120A (std::string port, int baudrate, int timeout = -1) :
		Device(port, baudrate, timeout, std::string("HP33120A")){
			reset();
			//_link.setTerminationString("\r");
	}
	
	std::string query(const std::string query, const int timeout);
	std::string getIDN();
	void reset();
	
	void setType(Type type);
	Type checkType();
	
	void setVoltageUnit(VoltageUnit unit);
	VoltageUnit checkVoltageUnit();
	
	void setFrequency(const float freq);
	void setAmplitude(const float ampl);
	void setOffset(const float offset);
	double getFrequency();
	double getAmplitude();
	double getOffset();
	
	

};

#ifdef __CINT__
#pragma link C++ class HP33120A-;
#endif

#endif
