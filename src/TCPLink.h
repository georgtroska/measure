// $Id$

// Copyright (C) 2009 Oliver Schulz <oliver.schulz@tu-dortmund.de>

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


#ifndef TCPLink_h
#define TCPLink_h

#include "stdint.h"

#include "DCELink.h"


namespace dce {


class TCPLink : public StreamPRLink {
public:


protected:
	void *m_socket;

	std::string m_hostname;
	uint16_t m_port;

	std::string m_uri;
	
public:
	std::string getCanonicalURI();
	std::string getURI();
	std::string getIdentity();
	std::string getDescription();
	
	bool isOpen() const;

	virtual void send(const void *buffer, size_t size, int timeout=-1) throw(std::runtime_error);
	virtual size_t recv(void *buffer, size_t size, int timeout=-1) throw(std::runtime_error);

	virtual void open(const std::string &uri) throw(std::invalid_argument, std::runtime_error);
	virtual void open(const std::string &hostname, uint16_t port) throw(std::invalid_argument, std::runtime_error);
	virtual void open(void *socket) throw(std::invalid_argument, std::runtime_error);
	virtual void close();
	
	TCPLink();
	//TCPLink(const std::string &uri) throw(std::runtime_error) throw(std::invalid_argument, std::runtime_error);
	TCPLink(const std::string &hostname, uint16_t port) throw(std::invalid_argument, std::runtime_error);
	virtual ~TCPLink();
};



class TCPServer {
	void *m_socket;

public:
	TCPLink* accept() throw(std::runtime_error);

	bool isOpen() const;
	
	void open(uint16_t port, unsigned int backlog=10) throw(std::invalid_argument, std::runtime_error);
	
	void close();

	TCPServer();
	TCPServer(uint16_t port, unsigned int backlog=10) throw(std::invalid_argument, std::runtime_error);

	virtual ~TCPServer();
};


} // namespace dce


#ifdef __CINT__
#pragma link C++ class dce::TCPServer-;
#pragma link C++ class dce::TCPLink-;
#endif


#endif // TCPLink_h
