#ifndef HP33120A_H
#define HP33120A_H

#include "Device.h"
#include "Param.h"

class HP33120A : public Device {
public:

	class Frequency : public Param {
		HP33120A & _dev;
		Device & getDevice() { return _dev; }
	public:
		Frequency(HP33120A * d) : _dev(*d), Param() { 
			_kind = "Frquency";
			_unit = "Hz";
			_saveName = "f";
		}

		virtual void operator()(double v) ;
		virtual double operator()();
		virtual void initialize() {};
	} freq;
	
	class Amplitude : public Param {
		HP33120A & _dev;
		Device & getDevice() { return _dev; }
	public:
		Amplitude(HP33120A * d) : _dev(*d), Param() { 
			_kind = "Voltage";
			_unit = "V";
			_saveName = "U";
		}
		virtual void operator()(double v) ;
		virtual double operator()();
		virtual void initialize() {};
	} ampl;
	
	class Offset : public Param {
		HP33120A & _dev;
		Device & getDevice() { return _dev; }
	public:
		Offset(HP33120A * d) : _dev(*d), Param() { 
			_kind = "OffsetVoltage";
			_unit = "V";
			_saveName = "U_0";
		}
		virtual void operator()(double v) ;
		virtual double operator()();
		virtual void initialize() {};
	} offset;
	
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
		Device(port, baudrate, timeout, std::string("HP33120A")), freq(this), ampl(this), offset(this){
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
