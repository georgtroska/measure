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


#include "PosixSocket.h"

#include <cassert>
#include <sstream>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <ext/stdio_filebuf.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;
using namespace dce;


namespace {

int stype2ptype(PosixSocketType type) throw(std::invalid_argument) {
	switch(type) {
		case SOCKTYPE_STREAM: return SOCK_STREAM;
		case SOCKTYPE_DGRAM: return SOCK_DGRAM;
		default: throw invalid_argument("Unknown socket type.");
	}
}

PosixSocketType ptype2stype(int type) throw(std::invalid_argument) {
	switch(type) {
		case SOCK_STREAM: return SOCKTYPE_STREAM;
		case SOCK_DGRAM: return SOCKTYPE_DGRAM;
		default: throw invalid_argument("Unknown socket type.");
	}
}

int sdom2pdom(PosixSocketDomain domain) throw(std::invalid_argument) {
	switch(domain) {
		case SOCKDOMAIN_LOCAL: return PF_LOCAL;
		case SOCKDOMAIN_INET: return PF_INET;
		default: throw invalid_argument("Unknown domain type.");
	}
}

// PosixSocketDomain pdom2sdom(int domain) throw(std::invalid_argument) {
// 	switch(domain) {
// 		case PF_LOCAL : return SOCKDOMAIN_LOCAL;
// 		case PF_INET : return SOCKDOMAIN_INET;
// 		default: throw invalid_argument("Unknown domain type.");
// 	}
// }

} // anonymous namespace



namespace dce {


PosixSocketAddr::PosixSocketAddr()
	: m_pSockAddr(new (struct sockaddr)),
	  m_pAddrLen(0), m_pFamily(0), m_pSockType(0), m_pProtocol(0),
      m_domain(SOCKDOMAIN_UNKNOWN), m_type(SOCKTYPE_UNKNOWN) {}

PosixSocketAddr::PosixSocketAddr(const std::string &hostName, unsigned int port,
	PosixSocketType sockType) throw(std::invalid_argument, std::runtime_error)
	: m_pSockAddr(new (struct sockaddr)),
	  m_pAddrLen(0), m_domain(SOCKDOMAIN_INET), m_type(sockType)
{
		int pDomain, pSockType;
		try {
			pDomain = sdom2pdom(m_domain);
			pSockType = stype2ptype(m_type);
		}
		catch (std::invalid_argument &e) { throw invalid_argument(e.what()); }

		int rc;
	
		struct addrinfo addrHints;
		struct addrinfo *addrInfo;
		
		memset(&addrHints, 0, sizeof(addrHints));
		addrHints.ai_family=pDomain;
		addrHints.ai_socktype=pSockType;
		
		rc = getaddrinfo(hostName.c_str(), 0, &addrHints, &addrInfo);
		if (rc!=0) {
			stringstream errMsg;
			errMsg << gai_strerror(rc) << ".";
			throw runtime_error(errMsg.str());
		}

		//uint32_t ipAddress = ((sockaddr_in*)addrInfo->ai_addr)->sin_addr.s_addr;
		((sockaddr_in*)addrInfo->ai_addr)->sin_port = htons(port); // Improve this - works only for IPv4 addresses!

		m_pFamily = addrInfo->ai_family;
		m_pSockType = addrInfo->ai_socktype;
		m_pProtocol = addrInfo->ai_protocol;
		
		struct sockaddr *saddr= (struct sockaddr*) m_pSockAddr;
		m_pAddrLen = addrInfo->ai_addrlen;
		assert( m_pAddrLen <= sizeof(*saddr) );
		//assert( sizeof(m_sockAddr) == sizeof(*addrInfo->ai_addr) );
		memcpy(saddr, addrInfo->ai_addr, m_pAddrLen);
		
		freeaddrinfo(addrInfo);
}


PosixSocketAddr::~PosixSocketAddr() {
	if (m_pSockAddr != 0) {
		struct sockaddr *saddr= (struct sockaddr*) m_pSockAddr;
		delete saddr;
	}
}



void PosixSocket::connectTo(const PosixSocketAddr &address) throw(std::runtime_error, std::runtime_error) {
	checkIfOpen();

	if (::connect(fd(), (const struct sockaddr*)address.posixSockAddrStruct(), address.posixSockAddrSize()) !=0 ) {
		stringstream errMsg;
		errMsg << "Can't connect: " << strerror(errno) << ".";
		throw runtime_error(errMsg.str());
	}
}


size_t PosixSocket::send(const void *data, size_t dataSize, int flags) throw(std::runtime_error, std::runtime_error) {
	checkIfOpen();
	ssize_t ret = ::send(fd(), data, dataSize, flags);
	if (ret < 0) throw runtime_error("Sending data failed.");
	return ret;
}

size_t PosixSocket::sendTo(const PosixSocketAddr &address, const void *data, size_t dataSize, int flags) throw(std::runtime_error, std::runtime_error) {
	checkIfOpen();
	ssize_t ret = ::sendto(fd(), data, dataSize, flags, (const struct sockaddr*)address.posixSockAddrStruct(), address.posixSockAddrSize());
	if (ret < 0) throw runtime_error("Sending data failed.");
	return ret;
}

size_t PosixSocket::recv(void *data, size_t dataSize, int flags) throw(std::runtime_error, std::runtime_error) {
	checkIfOpen();
	ssize_t ret = ::recv(fd(), data, dataSize, flags);
	if (ret < 0) throw runtime_error("Receiving data failed.");
	return ret;
}


void PosixSocket::listenTCP(uint16_t port, unsigned int backlog) throw(std::invalid_argument, std::runtime_error) {
	int pdomain = PF_INET;
	int ptype = SOCK_STREAM;
	int pproto = IPPROTO_TCP;
	
	int fd = ::socket(pdomain, ptype, pproto);
	if (fd < 0) throw runtime_error("Can't allocate network socket.");

	this->open(fd, true);
	
	sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons((u_short)port);

	int opt=1;
	if (setsockopt(this->fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
		string syserror(strerror(errno));
		throw runtime_error(string("Can't set socket options: "+syserror+"."));
	}

	if (bind(this->fd(), (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
		string syserror(strerror(errno));
		throw runtime_error(string("Can't bind socket to port: "+syserror+"."));
	}

	if (ptype == SOCK_STREAM) {
		if (listen(this->fd(), backlog) < 0)
			throw runtime_error("Can't listen on port.");
	}
}


PosixSocket* PosixSocket::accept() throw(std::invalid_argument, std::runtime_error) {
	int newfd = ::accept(fd(), 0, 0);
	if(newfd < 0) {
		string syserror(strerror(errno));
		throw runtime_error(string("Can't accept connection: "+syserror+"."));
	}
	
	return new PosixSocket(newfd, true);
}


void PosixSocket::open(int fd, bool closeFD) throw(std::invalid_argument, std::runtime_error) {
	if (isOpen()) throw invalid_argument("Can't open File/socket which is already open.");

	PosixFile::open(fd, closeFD);
	int pType;
	socklen_t pTypeSize = sizeof(pType);
	int rc = ::getsockopt(fd, SOL_SOCKET, SO_TYPE, &pType, &pTypeSize);
	if (rc == -1) throw runtime_error("Can't get socket options for file descriptor.");
	try { m_type = ptype2stype(pType); }
	catch (std::invalid_argument &e) { throw invalid_argument("File descriptor has unknown socket type."); }
}


PosixSocket::PosixSocket() {
}


PosixSocket::PosixSocket(int fd, bool closeFD) throw(std::invalid_argument, std::runtime_error) {
	this->open(fd, closeFD);
}


PosixSocket::PosixSocket(int family, int type, int protocol) throw(std::invalid_argument, std::runtime_error) {
	int fd = ::socket(family, type, protocol);
	if (fd < 0) throw runtime_error("Can't allocate network socket.");
	this->open(fd, true);
}


PosixSocket::PosixSocket(const PosixSocketAddr &address) throw(std::invalid_argument, std::runtime_error) {
	int fd = ::socket(address.posixFamily(), address.posixSockType(),
			address.posixProtocol());
	if (fd < 0) throw runtime_error("Can't allocate network socket.");
	this->open(fd, true);

	connectTo(address);
}


PosixSocket::~PosixSocket() {
}


} // namespace dce
