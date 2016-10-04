#include "K2000.h"

using namespace std;


void K2000::enable(bool on) {
}

void K2000::reset() {
}

bool K2000::isEnabled () {
	return true;
}

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

std::string K2000::getIDN() {
	_link.sendMsg("*IDN?\n");
	return _link.recvMsg(1000);
}

double K2000::getVoltageAC() {
	_link.sendMsg("MEAS:VOLT:AC?\n");
	cout << _link.recvMsg(1000);
	return 0;
}
