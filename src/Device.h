#ifndef DEVICE_H
#define DEVICE_H

#include "Signal.h"
#include "RS232Link.h"

#ifdef USEDAQCORE
#include <daqcore/VXI11Link.h>
#include <daqcore/TCPLink.h>
#endif

#include <iostream>
#include <string>
#include <cstdlib>


enum ConnectionType
{
	VXI11,
	SERIALLAN,
	RS232
};

/** Base class for all devices
* Basically the VXI11-Link and the signal class are provided here
*/

enum TriggerType {EXTERN,BUS,STANDARD};
enum ReadoutType {SINGLE,ALL};

class Device {
protected:
	//! A simple name
	std::string _name;
	
	//! The simple type in clear text
	std::string _type;
	
	//! The VXI11-Link
	class comLink {
#ifdef USEDAQCORE	

		dce::VXI11Link* vxi11;
		dce::TCPLink* tcp;
#endif
		dce::RS232Link *rs232;
		ConnectionType connectionType;

		std::string ip_for_reconnect;
		std::string adress_for_reconnect;

	public:
		comLink(){};
#ifdef USEDAQCORE	
		void connect(dce::VXI11Link ll) {
			vxi11 = &ll;
			connectionType = VXI11;
		};
		void connect(dce::TCPLink ll) {
			tcp = &ll;
			connectionType = SERIALLAN;
		};
		void connect(ConnectionType COMType, std::string ip, std::string adress) {
			connectionType = COMType;
			ip_for_reconnect = ip;
			adress_for_reconnect = adress;
			if(COMType==VXI11) {
				vxi11 = new dce::VXI11Link(ip,adress);
			} else if(COMType==SERIALLAN) {
				tcp = new dce::TCPLink(ip,atoi(adress.c_str()));
			}
		};
#endif
		void connect(ConnectionType COMType, std::string port, int baudrate, int timeout) {
			connectionType = COMType;
			if (COMType ==RS232) {
				rs232 = new dce::RS232Link(port, baudrate, timeout);
			}

		}
		void sendMsg(std::string msgtext, int timeout = -1) {
#ifdef USEDAQCORE	
			if(connectionType==VXI11) {
				vxi11->sendMsg(msgtext, timeout);
			} else if(connectionType==SERIALLAN) {
				try {
					tcp->sendStr(msgtext, timeout);
				}
				catch (...) {
					tcp->close();
					tcp->open(ip_for_reconnect,atoi(adress_for_reconnect.c_str()));
					tcp->sendStr(msgtext, timeout);
				}
			}
#endif
			if (connectionType==RS232) {
				rs232->sendMsg(msgtext,timeout);
			}
		};
		std::string recvMsg(int timeout = -1) {
			std::string answer;
#ifdef USEDAQCORE	
			if(connectionType==VXI11) {
				return vxi11->recvMsg(timeout);
			} else if(connectionType==SERIALLAN) {
				try {
					answer = tcp->recvStr(timeout);
				}
				catch (...) {
					tcp->close();
					tcp->open(ip_for_reconnect,atoi(adress_for_reconnect.c_str()));
					answer = tcp->recvStr(timeout);
				}
			}
			return answer;
#endif
			if (connectionType==RS232) {
				return rs232->recvLine(timeout);
			}
			return "";
			
		};
		size_t recv(void* buffer, size_t size, int timeout = -1){
			size_t length;
#ifdef USEDAQCORE	
			if(connectionType==VXI11) {
				return vxi11->recv(buffer, size, timeout);
			} else if(connectionType==SERIALLAN) {
				try {
					length = tcp->recv(buffer, size, timeout);
				}
				catch (...) {
					tcp->close();
					tcp->open(ip_for_reconnect,atoi(adress_for_reconnect.c_str()));
					length = tcp->recv(buffer, size, timeout);
				}
			}
			return length;
#endif
			if (connectionType==RS232) {
				return rs232->recv(buffer, size, timeout);
			}
			return 0;
		};
		void send(const void* buffer, size_t size, int timeout = -1){
#ifdef USEDAQCORE	
			if(connectionType==VXI11) {
				vxi11->send(buffer, size, timeout);
			} if(connectionType==SERIALLAN) {
				try {
					tcp->send(buffer, size, timeout);
				}
				catch (...) {
					tcp->close();
					tcp->open(ip_for_reconnect,atoi(adress_for_reconnect.c_str()));
					tcp->send(buffer, size, timeout);
				}
			}
#endif
			if (connectionType==RS232) {
				rs232->send(buffer, size, timeout);
			}
		};
		
		void setTerminationString(const std::string term) {
			if (connectionType==RS232) {
				rs232->setTerminationStringRecv(term);
			}
		};
	} _link;
	
	//! All signals are collected in this class
	Signal _signal;

	//! number of measurements into memory after trigger
	int _numberOfMeasurementsIntoMemoryAfterTrigger;

	int _waitingTimeBetweenConsecutiveMeasurements;

public:
	//Constructors
	/** Constructor with VXI11Link
	* @param link enter your VXI11Link here, if you have one already
	* @param name simple name for the device
	* @param type type of the device in clear text
	*/
#ifdef USEDAQCORE	
	Device ( dce::VXI11Link link, std::string name = "", std::string type = "" ):
		_name(name), _type(type) {
		_link.connect(link);
	}
	
	Device ( dce::TCPLink link, std::string name = "", std::string type = "" ):
		_name(name), _type(type) {
		_link.connect(link);
	}

	/** Constructor from scratch
	* @param ip the IP-adress of your device (VXI11 or GPIB) e.g. "gpib02.e4.physik.uni-dortmund.de"
	* @param gpib the gpib-adress of your device e.g. "gpib0,23"
	* @param name simple name for the device
	* @param type type of the device in clear text
	*/
	Device ( std::string ip, std::string gpib, std::string name = "", std::string type = "" ):
		_name(name), _type(type) {
		_link.connect(VXI11,ip,gpib);
	}
	
	Device ( ConnectionType comType, std::string ip, std::string gpib, std::string name = "", std::string type = "" ):
		_name(name), _type(type) {
		_link.connect(comType,ip,gpib);
	}
#endif

	Device ( std::string port, int baudrate, int timeout = -1, std::string name ="", std::string type = ""):
		_name(name), _type(type) {
		_link.connect(RS232,port,baudrate,timeout);
	}


	//FIXME: Needs a better way to solve this mess. Entangling low and high level functions is not appropriate.

	/** Standard Constructor
	* needed for Simulator-classes
	*/
	Device (){
	}
	
	virtual ~Device(){
		//_link.close();
	}
	
	/** Getter for the Name of the device
	* @return the name
	*/
	std::string getName() { return _name; }
	//Construtors end
	/** Getter for the SignalClass
	* @return the signal class
	*/
	virtual Signal & getSignal() {return _signal;}
	

	void sendMsg(std::string in, int timeout = -1) {
		_link.sendMsg(in,timeout);
	};
	
	std::string recvMsg(int timeout = -1){
		return _link.recvMsg(timeout);
	};
	
	size_t recv(void* buffer, size_t size, int timeout = -1){
		return _link.recv(buffer, size, timeout);
	};
	
	void send(const void* buffer, size_t size, int timeout = -1){
		_link.send(buffer, size, timeout);
	};

	virtual void setNumberOfConsecutiveMeasurements(unsigned int number){
		_numberOfMeasurementsIntoMemoryAfterTrigger = number;
	};
	
	virtual void setWaitingTimeBetweenConsecutiveMeasurements(unsigned int time){
		_waitingTimeBetweenConsecutiveMeasurements = time;
	};
	
	//@param time wait time between consecutive measurements in milliseconds
	virtual void setSpecialTriggerMode(TriggerType triggermode, ReadoutType readoutmode){
		std::cerr << "Method not supported." << std::endl;
	};
	
	virtual std::vector<double> readTriggeredDataAtOnce(){
		std::cerr << "Method not supported." << std::endl;
		return std::vector<double>();		
	};
};


#ifdef __CINT__
#pragma link C++ class Device-;
#endif

#endif
