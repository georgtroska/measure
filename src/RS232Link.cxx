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

using namespace std;

namespace dce {
std::string RS232Link::getCanonicalURI() {
	//!! Dummy implementation:
	return "";
}


std::string RS232Link::getURI() {
	return "";
}


std::string RS232Link::getIdentity() {
	return "";
}


std::string RS232Link::getDescription() {
	//!! Dummy implementation:
	return "";
}


bool RS232Link::isOpen() const {
	return m_device != -1;
}


void RS232Link::send(const void *buffer, size_t size, int timeout) throw(std::runtime_error) {
	checkIfOpen();
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


size_t RS232Link::recv(void *buffer, size_t size, int timeout) throw(std::runtime_error) {
	checkIfOpen();
	try {
		//sendMsg("?");
		
		size_t ret = ::read(m_device,buffer,size);
		return ret;
	} catch(...) {
		throw runtime_error("IO Error while receiving.");
	}
}


void RS232Link::open(const std::string &device, uint32_t baudrate, int timeout) throw(std::invalid_argument, std::runtime_error) {
	checkIfNotOpen();
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
	} catch(...) {
		throw runtime_error("Error while opening RS232 port");	
	}
}


void RS232Link::close() {
	checkIfOpen();
	
	/* restore the old port settings */
	tcsetattr(m_device,TCSANOW,&m_old);
	::close(m_device);
}


RS232Link::RS232Link()
	: m_device(-1)
{}
RS232Link::RS232Link(const std::string &device, uint32_t baudrate, int timeout) throw(std::invalid_argument, std::runtime_error) 
	: m_device(-1){
	open(device,baudrate,timeout);
	
}


RS232Link::~RS232Link() {
	try { if (isOpen()) close(); }
	catch (...) { assert( false ); }
}

void RS232Link::sendLine(const std::string line, int timeout) throw(std::runtime_error) {
	char out[1024];
	sprintf(out,"%s\r\n",line.c_str());
	sendMsg(out);
}

std::string RS232Link::recvLine(int timeout) throw(std::runtime_error) {
	std::string in;
	if (timeout == -1) timeout = m_timeout;
	bool isStrComplete=false;
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
		if (in.length() > 1) if (in.at(in.length()-1) == '\n') {
			break;
		}
		usleep(50000); //wait intervalls of 50ms
		timeout -= 50;
	} 
	/*
	if (isStrComplete) {
		in.erase(in.length()-2,2);	
	}
	*/ 
	return in;
}
std::string RS232Link::query(const std::string question, int timeout) throw(std::runtime_error) {
	sendLine(question);
	return recvLine(timeout);	
}




	
}

