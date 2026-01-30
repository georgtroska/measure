#ifndef HP33120A_H
#define HP33120A_H

#include "Device.h"
#include "Param.h"

class __declspec(dllexport) HP33120A : public Device {
public:

	class __declspec(dllexport) Frequency  : public Param {
		HP33120A & _dev;
		Device & getDevice() override { return (Device&)_dev; }
	public:
		Frequency(HP33120A * d) : _dev(*d), Param() {
			_kind = "Frquency";
			_unit = "Hz";
			_saveName = "f";
		}

		virtual void operator()(double v) override;
		virtual double operator()() override;
		virtual void initialize() override {};
	} freq;
	
	class __declspec(dllexport) Amplitude  : public Param {
		HP33120A & _dev;
		Device & getDevice() override { return (Device&)_dev; }
	public:
		Amplitude(HP33120A * d) : _dev(*d), Param() {
			/*_kind = "Voltage";
			_unit = "V";
			_saveName = "U";*/
		}
		virtual void operator()(double v) override;
		virtual double operator()() override;
		virtual void initialize() override {};
	} ampl;
	
	class __declspec(dllexport) Offset  : public Param {
		HP33120A & _dev;
		Device & getDevice() override { return (Device&)_dev; }
	public:
		Offset(HP33120A * d) : _dev(*d), Param() {
			/*_kind = "OffsetVoltage";
			_unit = "V";
			_saveName = "U_0";*/
		}
		virtual void operator()(double v) override;
		virtual double operator()() override;
		virtual void initialize() override {};
	} offset;
	
	enum __declspec(dllexport) Type {
		kSinusoid,
		kSquare,
		kTriangle,
		kRamp,
		kNoise,
		kDC,
		kUser,
		kUnknownType
	} _type;
	enum __declspec(dllexport) VoltageUnit {
		kVpp, //point to point
		kVRMS, //rms
		kVDBm, //DBm
		kUnknownUnit
	} _vunit;

public:
	HP33120A(std::string port, int baudrate, int timeout = -1); 
	HP33120A();
	/* :
		Device(port, baudrate, timeout, std::string("HP33120A")), freq(this), ampl(this), offset(this){
			reset();
			//_link.setTerminationString("\r");
	}*/
	
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

#ifdef __CLING__
#pragma link C++ class HP33120A-;
#pragma link C++ class HP33120A::Amplitude-;
#pragma link C++ class HP33120A::Frequency-;
#pragma link C++ class HP33120A::Offset-;
#endif

#endif
