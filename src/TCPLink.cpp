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


#include "TCPLink.h"

#include <iostream>
#include <cassert>
#include <cstring>
#include <sstream>

#include "PosixSocket.h"

using namespace std;


namespace dce {


std::string TCPLink::getCanonicalURI() {
	//!! Dummy implementation:
	return m_uri;
}


std::string TCPLink::getURI() {
	return m_uri;
}


std::string TCPLink::getIdentity() {
	return m_uri;
}


std::string TCPLink::getDescription() {
	//!! Dummy implementation:
	return "";
}


bool TCPLink::isOpen() const {
	return m_socket != 0;
}


void TCPLink::send(const void *buffer, size_t size, int timeout) throw(std::runtime_error) {
	checkIfOpen();

	PosixSocket *socket = (PosixSocket*) m_socket;
	try {
		//!! Implement timeout!
		size_t ret = socket->send(buffer, size);
		if (ret == size) return;
		else if (ret > size) throw runtime_error("Unexpected strange IO Error.");
		else if (ret > 0) throw runtime_error("Partial transmission while sending.");
		else if (ret == 0) throw runtime_error("Empty transmission while sending.");
	} catch(...) {
		throw runtime_error("IO Error while sending.");
	}
}


size_t TCPLink::recv(void *buffer, size_t size, int timeout) throw(std::runtime_error) {
	checkIfOpen();

	PosixSocket *socket = (PosixSocket*) m_socket;
	try {
		//!! Implement timeout!
		size_t ret = socket->recv(buffer, size);
		return ret;
	} catch(...) {
		throw runtime_error("IO Error while receiving.");
	}
}


void TCPLink::open(const std::string &uri) throw(std::invalid_argument, std::runtime_error) {
	//!! Dummy implementation:

	throw invalid_argument("Not implemented yet");
}


void TCPLink::open(const std::string &hostname, uint16_t port) throw(std::invalid_argument, std::runtime_error) {
	checkIfNotOpen();

	
	PosixSocketAddr address(hostname, port, SOCKTYPE_STREAM);
	assert(m_socket == 0);
	m_socket = new PosixSocket(address);
}


void TCPLink::open(void *socket) throw(std::invalid_argument, std::runtime_error) {
	checkIfNotOpen();

	m_socket = socket;
	if (! ((PosixSocket*) m_socket)->isOpen())
		throw invalid_argument("Can't open TCPLink from non-open socket.");
}


void TCPLink::close() {
	checkIfOpen();
	
	assert (m_socket != 0);
	delete ((PosixSocket*) m_socket); m_socket = 0;
}


TCPLink::TCPLink()
	: m_socket(0)
{}


//!!Implement!! TCPLink::TCPLink(const std::string &uri) throw(std::runtime_error) throw(std::invalid_argument, std::runtime_error);


TCPLink::TCPLink(const std::string &hostname, uint16_t port) throw(std::invalid_argument, std::runtime_error)
	: m_socket(0)
{
	open(hostname, port);
}


TCPLink::~TCPLink() {
	try { if (isOpen()) close(); }
	catch (...) { assert( false ); }
}


TCPLink* TCPServer::accept() throw(std::runtime_error) {
	if (! isOpen()) throw runtime_error("Server is not open/listening.");
	
	assert(m_socket != 0);
	PosixSocket *socket = (PosixSocket *) m_socket;
	PosixSocket* newSocket = socket->accept();
	TCPLink *newLink = new TCPLink();
	newLink->open(newSocket);
	return newLink;
}



bool TCPServer::isOpen() const {
	return (m_socket != 0);
}


void TCPServer::open(uint16_t port, unsigned int backlog) throw(std::invalid_argument, std::runtime_error) {
	if (isOpen()) throw runtime_error("Server is already open/listening.");
	
	assert(m_socket == 0);
	m_socket = new PosixSocket();
	PosixSocket *socket = (PosixSocket *) m_socket;
	socket->listenTCP(port, backlog);
}


void TCPServer::close() {
	if (!isOpen()) throw runtime_error("Server is not open/listening.");
	
	assert (m_socket != 0);
	delete ((PosixSocket*) m_socket); m_socket = 0;
}


TCPServer::TCPServer()
	: m_socket(0)
{}



TCPServer::TCPServer(uint16_t port, unsigned int backlog) throw(std::invalid_argument, std::runtime_error)
	: m_socket(0)
{
	open(port, backlog);
}


TCPServer::~TCPServer() {
	try { if (isOpen()) close(); }
	catch (...) { assert( false ); }
}


} // namespace dce
