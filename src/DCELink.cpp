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


#include "DCELink.h"
#include <iostream>

using namespace std;

namespace dce {


void DCELink::checkIfOpen() const {
	if (!isOpen()) throw runtime_error("Link is not open.");
}


void DCELink::checkIfNotOpen() const {
	if (isOpen()) throw runtime_error("Link is already open.");
}


	
void MessageLink::sendMsg(const std::string msg, int timeout)  {
	cout << "SENDING: " << msg << endl;
	send(msg.c_str(), msg.length(), timeout);
}


std::string MessageLink::recvMsg(int timeout)  {
	char buffer[1024];
	int res = recv(buffer, 1023, timeout);
	buffer[res] = 0;
	
	return string(buffer);
}


void StreamLink::sendStr(const std::string msg, int timeout) {
	send(msg.c_str(), msg.length(), timeout);
}


std::string StreamLink::recvStr(int timeout)  {
	char buffer[1024];
	int res = recv(buffer, 1023, timeout);
	buffer[res] = 0;
	return string(buffer);
}
	

} // namespace dce
