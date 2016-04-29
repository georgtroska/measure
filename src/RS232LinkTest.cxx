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
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.



#include <string>

#include <iostream>
#include <cstdlib>
#include <stdio.h>

#include "RS232Link.h"

//using namespace std;
using namespace dce;


int main (int argc, char *argv[], char *envp[]) {
	try {
		if (argc!=4) {
			std::cerr << "Syntax: " << argv[0] << " DEVICE BAUD MESSAGE" << std::endl;
			exit(1);
		}
		
		std::string device(argv[1]);
		std::string baud(argv[2]);
		std::string message(argv[3]);
		
		RS232Link link(device, atoi(baud.c_str()));
		link.sendLine(message);

		if (message.find("?") != std::string::npos) {
			std::string answer = link.recvLine(50000);
			std::cout << answer;
		}
	}
	catch(std::exception &e) {
		std::cerr << std::endl << std::endl << "Exception: " << e.what() << std::endl << std::endl;
		return 1;
	}
	
	return 0;
}
