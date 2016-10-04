#ifndef K2000_H
#define K2000_H

#include <cmath>

#include "Device.h"



/** This is the implementation for the Keithley 2000 Multimeter
 @code
 //Create a K2000 located at IP gpib01 under gpib-adress 16
 K2000 *K2000 = new K2000("gpib01","gpib0,16");
 @endcode
*/

class K2000: public Device {
public:
#ifdef USEDAQCORE	
	//Constructors
	/** Constructor for K2000 with VXI11Link
	* @param link enter your VXI11Link here, if you have one already
	* @param name simple name for the device
	*/
	K2000(dce::VXI11Link link, std::string name = "K2000"):
		Device(link, name, std::string("K2000")){
			reset();
	}
	/** Constructor for K2000 from scratch
	* @param ip the IP-adress of your device (VXI11 or GPIB) e.g. "gpib02.e4.physik.uni-dortmund.de"
	* @param gpib the gpib-adress of your device e.g. "gpib0,23"
	* @param name simple name for the device
	*/
	K2000(std::string ip, std::string gpib, std::string name = "K2000"):
		Device(ip, gpib, name, std::string("K2000")){
			reset();
	}
#endif

	K2000(std::string port, int baudrate, int timeout = -1) :
		Device(port, baudrate, timeout, std::string("K2000")){
			reset();
	}
	//Constructors end

	/** Resets device to factory defaults and enables the device (Turns operate on and Zero-Check off)
	* @param on set to false to disable
	*/
	void enable(bool on = true);

	/** Resets device to factory defaults
	*
	*/
	void reset();

	/** Gets the status of the device
	* @return true when operating
	*/
	bool isEnabled ();

	double getResistance(int channel);

	double getResistance_4Wire(int channel);

	double getTemperature(int channel);

	double getTemperature_4Wire(int channel);

	double getVoltage(int channel);

	double getHumidity(int channel);
	
	std::string getIDN();
};

#ifdef __CINT__
#pragma link C++ class K2000-;
#endif

#endif
