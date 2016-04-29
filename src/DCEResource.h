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


#ifndef DCEResource_h
#define DCEResource_h

#include <stdexcept>

#include "SmartPtr.h"


namespace dce {


class Timeout : public std::runtime_error {
public:
	Timeout();
};

	
/// Basic DAqCorE resource class

class DCEResource {
public:
	virtual std::string getURI() = 0; //!! Make const! Return reference?
	virtual std::string getIdentity() = 0; //!! Make const! Return reference?
	virtual std::string getDescription() = 0; //!! Make const! Return reference?
	
	virtual ~DCEResource();
};


} // namespace dce


#ifdef __CINT__
#pragma link C++ class dce::Timeout-;
#pragma link C++ class dce::DCEResource-;
#pragma link C++ class dce::SmartPtr<dce::DCEResource>-;
#endif

#endif // DCELink_h
