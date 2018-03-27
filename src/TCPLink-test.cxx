// $Id$

// Copyright (C) 2003-2008 Oliver Schulz <oliver.schulz@tu-dortmund.de>

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


#include "SmartPtr.h"
#include "TCPLink.h"

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace dce;


int main (int argc, char *argv[], char *envp[]) {
	try {
		cout << "Waiting for connection on port 9003" << endl;
		
		// Start server:
		TCPServer server(9002);

		// Initiate client connection to server:
		TCPLink clink("localhost", 9002);

		// Server accepts and sends message:
		SmartPtr<TCPLink> slink = server.accept();
		slink->sendStr("Hello, client!\n");

		// Client receives and sends response:
		cout << clink.recvStr();
		clink.sendStr("Hello, server!\n");

		// Server receives client response:
		cout << slink->recvStr();
	}
	catch(std::exception &e) {
		cerr << endl << endl << "Exception: " << e.what() << endl << endl;
		return 1;
	}
	
	return 0;
}
