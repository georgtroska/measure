#include "K2000.h"

using namespace std;


void K2000::enable(bool on) {
}

void K2000::reset() {
}

bool K2000::isEnabled () {
	return true;
}
/*

double K2000::getResistance(int channel){
	std::string in;
	char close_channel[8];
	sprintf(close_channel,"N%i", channel);
	char kind[10]="";
	char out[255]="";

	_link.sendMsg("F2");
	usleep(10000);
	_link.sendMsg("R2");
	usleep(10000);
	_link.sendMsg(close_channel);
	usleep(10000);
	_link.sendMsg("X");
	usleep(10000);
	_link.sendMsg("?");
	in=_link.recvMsg();
	in.copy(kind,4,0);
	in.copy(out,16,4);
	return atof(out);
}

double K2000::getResistance_4Wire(int channel){
	std::string in;
	char close_channel[8];
	sprintf(close_channel,"N%i", channel);
	char kind[10]="";
	char out[255]="";

	_link.sendMsg("F9");
	usleep(10000);
	_link.sendMsg("R2");
	usleep(10000);
	_link.sendMsg(close_channel);
	usleep(10000);
	_link.sendMsg("X");
	usleep(10000);
	_link.sendMsg("?");
	in=_link.recvMsg();
	in.copy(kind,4,0);
	in.copy(out,16,4);
	return atof(out);
}

double K2000::getTemperature(int channel){
	double R = getResistance(channel);
	const double a=3.90802E-3;
    const double b=-5.802E-7;
    const double R0=1000.00;

    return -a/(2*b)-sqrt(R/(R0*b)-1/b+(a/(2*b))*(a/(2*b)));
}

double K2000::getTemperature_4Wire(int channel){
	double R = getResistance_4Wire(channel);
	const double a=3.90802E-3;
    const double b=-5.802E-7;
    const double R0=1000.00;

    return -a/(2*b)-sqrt(R/(R0*b)-1/b+(a/(2*b))*(a/(2*b)));
}

double K2000::getVoltage(int channel){
	std::string in;
	char close_channel[8];
	sprintf(close_channel,"N%i",channel);
	char kind[10]="";
	char out[255]="";

	_link.sendMsg("F0");
	usleep(10000);
	_link.sendMsg("R0");
	usleep(10000);
	_link.sendMsg(close_channel);
	usleep(10000);
	_link.sendMsg("X");
	usleep(10000);
	_link.sendMsg("?");
	in=_link.recvMsg();
	in.copy(kind,4,0);
	in.copy(out,16,4);
	return atof(out);
}

double K2000::getHumidity(int channel){
	double voltage = getVoltage(channel);
	double temperature = getTemperature(channel);
	const double a=0.0315;
	const double b=0.826;

	return ((-voltage-b)/a)/(1.0546-0.00216*temperature);
}
*/	
std::string K2000::getIDN() {
	_link.sendMsg("*IDN?\n");
	usleep(100000);
	return _link.recvMsg(1000);
}
/*
double K2000::getVoltageAC() {
	_link.sendMsg("FETC?\n");
	//_link.sendMsg("MEAS:VOLT:AC?\n");
	cout << _link.recvMsg(1000);
	return 0;
}
*/
std::string K2000::query(const std::string query, const int timeout) {
	_link.sendMsg(query);
	return _link.recvMsg(timeout);
}

void K2000::setMode(const K2000::Mode mode) {
	std::string sendStr = "";
	if (mode == kUnknown) {
		cerr << "Impossible to set mode to kUnknown" << endl;
		return;
	}
	switch (mode) {
		case kCurrentAC:
			sendStr = "CONF:CURR:AC\n";
			break;
		case kCurrentDC:
			sendStr = "CONF:CURR\n";
			break;
		case kVoltageAC:
			sendStr = "CONF:VOLT:AC\n";
			break;
		case kVoltageDC:
			sendStr = "CONF:VOLT\n";
			break;
		case kResistance2:
			sendStr = "CONF:RES\n";
			break;
		case kResistance4:
			sendStr = "CONF:FRES\n";
			break;
		case kPeriod:
			sendStr = "CONF:PER\n";
			break;
		case kFrequency:
			sendStr = "CONF:FREQ\n";
			break;
		case kTemperature:
			sendStr = "CONF:TEMP\n";
			break;
		case kDiode:
			sendStr = "CONF:DIOD\n";
			break;
		case kContinuity:
			sendStr = "CONF:CONT\n";
			break;
	}
	query(sendStr,1000);
	_mode = checkMode();
	if (_mode != mode) {
		cerr << "Not possible to set correct mode!" << endl;
		return;
	}
	_link.sendMsg("INIT:CONT 1\n"); //Enable continous measuring, no dedicated triggering
	
}

K2000::Mode K2000::checkMode() {
	std::string myMode = query("CONF?\n",1000);
	if (myMode.find("VOLT:DC") != std::string::npos) {
		return kVoltageDC;
	} else if (myMode.find("VOLT:AC") != std::string::npos) {
		return kVoltageAC;
	} else  if (myMode.find("CURR:DC") != std::string::npos) {
		return kCurrentDC;
	} else if (myMode.find("CURR:AC") != std::string::npos) {
		return kCurrentAC;
	} else if (myMode.find("FRES") != std::string::npos) {
		return kResistance4;
	} else if (myMode.find("RES") != std::string::npos) {
		return kResistance2;
	} else if (myMode.find("PER") != std::string::npos) {
		return kPeriod;
	} else if (myMode.find("FREQ") != std::string::npos) {
		return kFrequency;
	} else if (myMode.find("TEMP") != std::string::npos) {
		return kTemperature;
	} else if (myMode.find("DIOD") != std::string::npos) {
		return kDiode;
	} else if (myMode.find("CONT") != std::string::npos) {
		return kContinuity;
	} else {
		return kUnknown;
	}
}

