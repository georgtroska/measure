// $Id$

// Copyright (C) 2004-2009 Oliver Schulz <oliver.schulz@tu-dortmund.de>

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


#ifndef PosixSocket_h
#define PosixSocket_h

#include <iostream>
#include <string>
#include <stdint.h>

#include "PosixFile.h"


namespace dce {


enum PosixSocketDomain {
	SOCKDOMAIN_UNKNOWN = 0,
	SOCKDOMAIN_LOCAL = 1,
	SOCKDOMAIN_INET = 2
};


enum PosixSocketType {
	SOCKTYPE_UNKNOWN = 0,
	SOCKTYPE_STREAM = 1,
	SOCKTYPE_DGRAM = 2
};


class PosixSocketAddr {
protected:
	void *m_pSockAddr;
	size_t m_pAddrLen;

	int m_pFamily;
    int m_pSockType;
	int m_pProtocol;
	
	PosixSocketDomain m_domain;
	PosixSocketType m_type;
	
public:
	int posixFamily() const { return m_pFamily; }
	int posixSockType() const { return m_pSockType; }
	int posixProtocol() const { return m_pProtocol; }
	void* posixSockAddrStruct() const { return m_pSockAddr; }
	size_t posixSockAddrSize() const { return m_pAddrLen; }
	
	PosixSocketDomain domain() const { return m_domain; }
	PosixSocketType type() const { return m_type; }
	
	PosixSocketAddr();
	PosixSocketAddr(const std::string &hostName, unsigned int port, PosixSocketType sockType);

	virtual ~PosixSocketAddr();
};


class PosixSocket : public PosixFile {
protected:
	PosixSocketType m_type;

public:
	PosixSocketType type() const { return m_type; }
	
	void connectTo(const PosixSocketAddr &address);

	size_t send(const void *data, size_t dataSize, int flags = 0);

	size_t sendTo(const PosixSocketAddr &address, const void *data, size_t dataSize, int flags = 0);

	size_t recv(void *data, size_t dataSize, int flags = 0);

	//ssize_t recvFrom(const PosixSocketAddr &address, void *data, size_t dataSize, int flags = 0) throw(std::runtime_error, std::runtime_error);
		// Not implemented yet.

	//void bind(const PosixSocketAddr &address);
		// Not implemented yet.

	void listenTCP(uint16_t port, unsigned int backlog=10);

	PosixSocket* accept();

	//PosixSocket accept(PosixSocketAddr &address); //!! Return type problems
		// Not implemented yet.
		
	virtual void open(int fd, bool closeFD=true);

	PosixSocket();

	PosixSocket(int fd, bool closeFD=true);
	
	PosixSocket(int family, int type, int protocol = 0);

	PosixSocket(const PosixSocketAddr &address);

	virtual ~PosixSocket();
};


} // namespace dce


#ifdef __CLING__
//#pragma link C++ class dce::PosixSocketAddr-;
//#pragma link C++ class dce::PosixSocket-;
#endif


#endif // PosixSocket_h
