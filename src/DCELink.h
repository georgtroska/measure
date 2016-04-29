// $Id$

// Copyright (C) 2008-2009 Oliver Schulz <oliver.schulz@tu-dortmund.de>

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
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#ifndef DCELink_h
#define DCELink_h

#include <stdexcept>
#include "DCEResource.h"


namespace dce {
	

/// Basic DAqCorE communicaton-link class

class DCELink : public DCEResource {
protected:
	void checkIfOpen() const throw(std::runtime_error);

	void checkIfNotOpen() const throw(std::runtime_error);

public:
	virtual std::string getCanonicalURI() = 0;

	virtual bool isOpen() const = 0;

	// virtual void open(const std::string &uri) throw(std::invalid_argument, std::runtime_error) = 0;
	// virtual void close() = 0;
};



/// Message send/receive link

class MessageLink : public DCELink {
protected:
public:
	virtual void send(const void *buffer, size_t bufsize, int timeout=-1) throw(std::runtime_error) = 0;
	virtual size_t recv(void *buffer, size_t bufsize, int timeout=-1) throw(std::runtime_error) = 0;

	virtual void sendMsg(const std::string msg, int timeout=-1) throw(std::runtime_error);
	virtual std::string recvMsg(int timeout=-1) throw(std::runtime_error);
};


/// Message send/receive link

class StreamLink : public DCELink {
protected:
public:
	virtual void send(const void *buffer, size_t bufsize, int timeout=-1) throw(std::runtime_error) = 0;
	virtual size_t recv(void *buffer, size_t bufsize, int timeout=-1) throw(std::runtime_error) = 0;

	virtual void sendStr(const std::string msg, int timeout=-1) throw(std::runtime_error);
	virtual std::string recvStr(int timeout=-1) throw(std::runtime_error);
};


/// Message link, active reception
///
/// Examples: RS485

class StreamARLink : public StreamLink {
protected:
public:
};


/// Message link, passive reception
///
/// Examples: TCP/IP, UDP/IP, RS232, RS422

class StreamPRLink : public StreamLink {
protected:
public:
};


/// Message link, transactional
///
/// Examples: I2C

class MsgTALink : public MessageLink {
protected:
public:
	virtual size_t transact(const void *txBuf, size_t txSize, void *rxBuf, size_t rxSize, int timeout) = 0;
};


/// Data-Exchange link
///
/// Examples: SPI

class DataExchLink : public DCELink {
protected:
public:
	virtual size_t dxch(const void *txBuf, size_t txSize, void *rxBuf, size_t rxSize, int timeout) = 0;
};


/// Memory/Register-Style link

class MemoryLink : public DCELink {
protected:
public:
	virtual void write(size_t address, const void *buffer, size_t bufsize) throw(std::runtime_error) = 0;
	virtual size_t read(size_t address, void *buffer, size_t bufsize) throw(std::runtime_error) = 0;
};


} // namespace dce


#ifdef __CINT__
#pragma link C++ class dce::Timeout-;
#pragma link C++ class dce::DCEResource-;
#pragma link C++ class dce::SmartPtr<dce::DCEResource>-;
#pragma link C++ class dce::DCELink-;
#pragma link C++ class dce::SmartPtr<dce::DCELink>-;
#pragma link C++ class dce::MessageLink-;
#pragma link C++ class dce::SmartPtr<dce::MessageLink>-;
#pragma link C++ class dce::StreamLink-;
#pragma link C++ class dce::SmartPtr<dce::StreamLink>-;
#pragma link C++ class dce::StreamARLink-;
#pragma link C++ class dce::SmartPtr<dce::StreamARLink>-;
#pragma link C++ class dce::StreamPRLink-;
#pragma link C++ class dce::SmartPtr<dce::StreamPRLink>-;
#pragma link C++ class dce::MsgTALink-;
#pragma link C++ class dce::SmartPtr<dce::MsgTALink>-;
#pragma link C++ class dce::DataExchLink-;
#pragma link C++ class dce::SmartPtr<dce::DataExchLink>-;
#pragma link C++ class dce::MemoryLink-;
#pragma link C++ class dce::SmartPtr<dce::MemoryLink>-;
#endif

#endif // DCELink_h
