#include "HP33120A.h"
#include <iostream>

using namespace std;

HP33120A::HP33120A(std::string port, int baudrate, int timeout) :
	Device(port, baudrate, timeout, std::string("HP33120A")), freq(this), ampl(this), offset(this) {
	reset();
	//_link.setTerminationString("\r");
}
HP33120A::HP33120A() :
	Device(), freq(this), ampl(this), offset(this) {

}

std::string HP33120A::query(const std::string query, const int timeout) {
	_link.sendMsg(query);
	return _link.recvMsg(timeout);
}

std::string HP33120A::getIDN() {
	return query("*IDN?\n",1000);
}

void HP33120A::reset() {
}

void HP33120A::setType(HP33120A::Type type) {
	std::string sendStr = "";
	if (type == kUnknownType) {
		cerr << "Impossible to set type to kUnknown" << endl;
		return;
	}
	switch (type) {
		case kSinusoid:
			sendStr = "FUNC:SHAP SIN\n";
			break;
		case kSquare:
			sendStr = "FUNC:SHAP SQU\n";
			break;
		case kTriangle:
			sendStr = "FUNC:SHAP TRI\n";
			break;
		case kRamp:
			sendStr = "FUNC:SHAP RAMP\n";
			break;
		case kNoise:
			sendStr = "FUNC:SHAP NOIS\n";
			break;
		case kDC:
			sendStr = "FUNC:SHAP DC\n";
			break;
		case kUser:
			sendStr = "FUNC:SHAP USER\n";
			break;
	}
	query(sendStr,1000);
	_type = checkType();
	/*
	if (_type != type) {
		cerr << "Not possible to set correct type!" << endl;
		return;
	}
	*/
	
}

HP33120A::Type HP33120A::checkType() {
	std::string myType = query("FUNC:SHAP?\n",5000);
	if (myType.find("SIN") != std::string::npos) {
		return kSinusoid;
	} else if (myType.find("SQU") != std::string::npos) {
		return kSquare;
	} else  if (myType.find("TRI") != std::string::npos) {
		return kTriangle;
	} else if (myType.find("RAMP") != std::string::npos) {
		return kRamp;
	} else if (myType.find("NOIS") != std::string::npos) {
		return kNoise;
	} else if (myType.find("DC") != std::string::npos) {
		return kDC;
	} else if (myType.find("USER") != std::string::npos) {
		return kUser;
	} else {
		return kUnknownType;
	}
}

void HP33120A::setVoltageUnit(HP33120A::VoltageUnit unit) {
	std::string sendStr = "";
	if (unit == kUnknownUnit) {
		cerr << "Impossible to set Unit to kUnknown" << endl;
		return;
	}
	switch (unit) {
		case kVpp:
			sendStr = "VOLT:UNIT VPP\n";
			break;
		case kVRMS:
			sendStr = "VOLT:UNIT VRMS\n";
			break;
		case kVDBm:
			sendStr = "VOLT:UNIT DBM\n";
			break;
		
	}
	query(sendStr,1000);
	/*
	_vunit = checkVoltageUnit();
	if (_vunit != unit) {
		cerr << "Not possible to set correct type!" << endl;
		return;
	}
	*/
}

HP33120A::VoltageUnit HP33120A::checkVoltageUnit() {
	std::string myType = query("VOLT:UNIT?\n",5000);
	if (myType.find("VPP") != std::string::npos) {
		return kVpp;
	} else if (myType.find("VRMS") != std::string::npos) {
		return kVRMS;
	} else  if (myType.find("VDBM") != std::string::npos) {
		return kVDBm;
	} else {
		return kUnknownUnit;
	}
}

void HP33120A::setFrequency(const float freq) {
	char out[64];
	sprintf(out,"FREQ %f\n",freq);
	query(std::string(out),100);
}

void HP33120A::setAmplitude(const float ampl) {
	char out[64];
	sprintf(out,"VOLT %f\n",ampl);
	query(std::string(out),100);
}

void HP33120A::setOffset(const float offset) {
	char out[64];
	sprintf(out,"VOLT:OFFS %f\n",offset);
	query(std::string(out),100);
}

double HP33120A::getFrequency() {
	return atof(query("FREQ?\n",1000).c_str());
}

double HP33120A::getAmplitude() {
	return atof(query("VOLT?\n",1000).c_str());
}

double HP33120A::getOffset() {
	return atof(query("VOLT:OFFS?\n",1000).c_str());
}

void HP33120A::Frequency::operator()(double f){
	_dev.setFrequency(f);
}

double HP33120A::Frequency::operator()() {
	return _dev.getFrequency();
}

void HP33120A::Amplitude::operator()(double a){
	_dev.setAmplitude(a);
}

double HP33120A::Amplitude::operator()() {
	return _dev.getAmplitude();
}

void HP33120A::Offset::operator()(double u){
	_dev.setOffset(u);
}

double HP33120A::Offset::operator()() {
	return _dev.getOffset();
}
