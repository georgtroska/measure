// $Id$

// Copyright (C) 2009 Georg Troska <georg.troska@tu-dortmund.de>

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software

#include "RS232Link.h"

#include <iostream>
#include <cassert>
#include <cstring>
#include <sstream>

#include <fcntl.h>
#include <sys/ioctl.h>

using namespace std;

std::string dce::RS232Link::getCanonicalURI() {
	//!! Dummy implementation:
	return "";
}


std::string dce::RS232Link::getURI() {
	return "";
}


std::string dce::RS232Link::getIdentity() {
	return "";
}


std::string dce::RS232Link::getDescription() {
	//!! Dummy implementation:
	return "";
}


bool dce::RS232Link::isOpen() const {
	return m_device != -1;
}


void dce::RS232Link::send(const void *buffer, size_t size, int timeout) throw(std::runtime_error) {
	//checkIfOpen();
	try {
		size_t ret = ::write(m_device,buffer,size);
		if (ret == size) return;
		else if (ret > size) throw runtime_error("Unexpected strange IO Error.");
		else if (ret > 0) throw runtime_error("Partial transmission while sending.");
		else if (ret == 0) throw runtime_error("Empty transmission while sending.");
	} catch(...) {
		throw runtime_error("IO Error while sending.");
	}
}

size_t dce::RS232Link::recv(void *buffer, size_t size, int timeout) throw(std::runtime_error) {
	checkIfOpen();
	try {
		//sendMsg("?");
		size_t ret = ::read(m_device,buffer,size);
		return ret;
	} catch(...) {
		throw runtime_error("IO Error while receiving.");
	}
}



void dce::RS232Link::open(const std::string &device, uint32_t baudrate, int timeout) throw(std::invalid_argument, std::runtime_error) {
	//checkIfNotOpen();
	try {
		m_device = ::open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
		
		if (m_device == -1) {}//throw runtime_error("Unable to open RS232 port");
		m_timeout = timeout;		
		
		
		fcntl(m_device, F_SETFL,0);
		// Store the current settings.
		tcgetattr(m_device,&m_old);
		// Setup place for new settings.
		tcgetattr(m_device, &m_options);
		// Insure not owner of port (CLOCAL) enable reading(CREAD).
		m_options.c_cflag |= (CLOCAL | CREAD);
		// Set parity, stop bit, bit size (8N1).
		m_options.c_cflag &= ~PARENB;
		m_options.c_cflag &= ~CSTOPB;
		m_options.c_cflag &= ~CSIZE;
		m_options.c_cflag |= CS8;
		// Disable hardware flow control.
		m_options.c_cflag &= ~CRTSCTS;
		
		// Choose Raw Input Input characters are passed through exactly as
		// they are received, when they are received.
		m_options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
		// Disable software flow control.
		m_options.c_iflag &= ~(IXON | IXOFF | IXANY);
		// Choose Raw Output, all other option bits in  c_oflag are ignored.
		m_options.c_oflag &= ~OPOST;
		
		
		//no timeout here .... I will do it in software
		m_options.c_cc[VMIN] = 0; 
		m_options.c_cc[VTIME] = 0;
		
		cfsetispeed(&m_options, baudrate);
		cfsetospeed(&m_options, baudrate);
		                             
         tcflush(m_device, TCIFLUSH);  
		
		
		tcsetattr(m_device, TCSANOW, &m_options);
		/*
		int serial;
		ioctl(m_device, TIOCMGET, &serial);
           if (serial & TIOCM_DTR)
               std::cout << "TIOCM_DTR is set" << std::endl;
           else
               std::cout << "TIOCM_DTR is not set" << std::endl;
               */
	} catch(...) {
		throw runtime_error("Error while opening RS232 port");	
	}
}


void dce::RS232Link::close() {
	//checkIfOpen();
	
	/* restore the old port settings */
	tcsetattr(m_device,TCSANOW,&m_old);
	::close(m_device);
}


dce::RS232Link::RS232Link()
	: m_device(-1)
{}
dce::RS232Link::RS232Link(const std::string &device, uint32_t baudrate, int timeout) throw(std::invalid_argument, std::runtime_error) 
	: m_device(-1), _terminationStringSend("\n"), _terminationStringRecv("\n"), _delayAfterSend(-1) {
	open(device,baudrate,timeout);
	
}


dce::RS232Link::~RS232Link() {
	try { if (isOpen()) close(); }
	catch (...) { assert( false ); }
}

void dce::RS232Link::sendLine(const std::string line, int timeout) throw(std::runtime_error) {
	char out[1024];
	sprintf(out,"%s%s",line.c_str(),_terminationStringSend.c_str());
	sendMsg(out, timeout);
	int bytes;
	int myDelay = _delayAfterSend;
	while (1) {
		ioctl(m_device, TIOCOUTQ, &bytes);
		usleep(50); //50us
		if (bytes == 0) {
			myDelay -= 50;
			if (myDelay < 0) break;
		}
	}
}

std::string dce::RS232Link::recvLine(int timeout) throw(std::runtime_error) {
	std::string in;
	int iFlags;
/*
	// turn off DTR
	iFlags = TIOCM_DTR;
	ioctl(m_device, TIOCMBIC, &iFlags);
	usleep(10000); //wait intervalls of 50ms
			// turn on DTR
	iFlags = TIOCM_DTR;
	ioctl(m_device, TIOCMBIS, &iFlags);
	usleep(25000); //wait intervalls of 50ms
	// turn off DTR
	iFlags = TIOCM_DTR;
	ioctl(m_device, TIOCMBIC, &iFlags);
	*/
	if (timeout == -1) timeout = m_timeout;
	bool isStrComplete=false;
	size_t found = std::string::npos;
	while (timeout > 1 | timeout == -1) {
		in += recvMsg();
		/*if (in.length() > 2) if (in.at(in.length()-3) == '\r' && in.at(in.length()-2) == '\r'&& in.at(in.length()-1) == '\n') continue;
		if (in.length() > 1) {
		if (in.at(in.length()-2) == '\r' && in.at(in.length()-1) == '\n') {
			isStrComplete = true;	
			break;
		}
		}
		*/
		
		if (in.length() > 0) {
			found = in.find(_terminationStringRecv);
			
			if (found != std::string::npos) { //found termination string
				isStrComplete = true;
				break;
			}
		}
		usleep(50000); //wait intervalls of 50ms
		timeout -= 50;
	} 
	/*
		// turn on DTR
	iFlags = TIOCM_DTR;
	ioctl(m_device, TIOCMBIS, &iFlags);
*/
	
	if (in.length() > 2) {
		cout << "Received: " << in << endl;
		cout << "Last chars are: 0x" << hex << (int)in.at(in.length()-2) << " 0x" << (int)in.at(in.length()-1) << endl;
	}
	
	if (isStrComplete) {
		//in.erase(in.length()-2,2);
		return std::string(in,0,found);
	}
	return in;
}
std::string dce::RS232Link::query(const std::string question, int timeout) throw(std::runtime_error) {
	sendLine(question);
	return recvLine(timeout);	
}






	


