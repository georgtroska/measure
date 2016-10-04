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

#ifndef RS232Link_h
#define RS232Link_h	

#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>

#include "DCELink.h"

namespace dce {

class RS232Link : public MessageLink{
public:
	
protected:
	int m_device;
	struct termios m_old, m_options;
	
	int m_timeout; //in milliseconds;
	
	std::string _terminationStringSend;
	std::string _terminationStringRecv;
	int _delayAfterSend;
	
	
	
public:

	std::string getCanonicalURI();
	std::string getURI();
	std::string getIdentity();
	std::string getDescription();
	
	bool isOpen() const;
	
	void setTerminationStringSend(const std::string str) { _terminationStringSend = str; }
	void setTerminationStringRecv(const std::string str) { _terminationStringRecv = str; }
	void setDelayAfterSend(const int delay) { _delayAfterSend = delay; } //in microseconds

	virtual void send(const void *buffer, size_t size, int timeout=-1) throw(std::runtime_error);
	virtual size_t recv(void *buffer, size_t size, int timeout=-1) throw(std::runtime_error);
	///! timeout in units of 0.1s
	virtual void open(const std::string &device, uint32_t baudrate, int timeout) throw(std::invalid_argument, std::runtime_error);
	
	virtual void close();
	
	RS232Link();
	RS232Link(const std::string &device, uint32_t baudrate, int timeout=1) throw(std::invalid_argument, std::runtime_error);
	virtual ~RS232Link();
	virtual void sendLine(const std::string line, int timeout=-1) throw(std::runtime_error);
	virtual std::string recvLine(int timeout=-1) throw(std::runtime_error);
	virtual std::string query(const std::string question, int timeout = -1) throw(std::runtime_error);
	
};

}



#ifdef __CINT__
#pragma link C++ class dce::RS232Link-;
#endif


#endif
