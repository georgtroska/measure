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


#ifndef PosixFile_h
#define PosixFile_h

#include <stdexcept>
#include <string>


namespace dce {


class PosixFile {
protected:
	int m_fd;
	bool m_closeFD;
	
	void checkIfOpen() const;

	void setFileFlag(int flag, bool value);
	
public:
	enum AccessMode {
		ACC_READ = 1,    // Open for reading
		ACC_WRITE = 2,   // Open for writing
		ACC_RDWR = 3,    // Open for reading and writing
		ACC_APPEND = 4   // Open for appending only
	};

	enum CreateMode {
		CREA_NONE = 1,   // Do not create file.
		CREA_CREATE = 2, // Create file if not existing.
		CREA_TRUNC = 3,  // Create and truncate file if not existing.
		CREA_EXCL = 4    // Create file, error if already existing.
	};

	int fd() const { return m_fd; }

	bool isOpen() const { return m_fd >= 0; }
	
	// File has to be closed before re-opening.
	// Use an already existing FD. set closeFD to false if that FD
	// should not be closed on close() or ~PosixFile().
	virtual void open(int fd, bool closeFD=true);
	
	virtual void close();

	void blocking(bool block);

	bool pollRead(int timeout);

	bool pollWrite(int timeout);
	
	size_t read(void *buffer, size_t bufferSize);

	size_t write(const void *buffer, size_t bufferSize);
	
	void sync();
	
	PosixFile();

	// PosixFile(const std::string &fileName, AccessMode accMode, CreateMode creaMode=CREA_NONE, bool nonblock=false, bool async=false) throw(std::invalid_argument, std::runtime_error);

	PosixFile(int fd, bool closeFD=true);

	virtual ~PosixFile();
};


class FSPosixFile : public PosixFile {
public:
	// File has to be closed before re-opening.
	virtual void open(const std::string &fileName, AccessMode accMode, CreateMode creaMode);

	// File has to be closed before re-opening.
	virtual void open(const std::string &fileName, AccessMode accMode, CreateMode creaMode, bool nonblock, bool async=false);
	
	FSPosixFile();

	FSPosixFile(const std::string &fileName, AccessMode accMode, CreateMode creaMode=CREA_NONE, bool nonblock=false, bool async=false);

	virtual ~FSPosixFile();
};


} // namespace dce


#endif // PosixFile_h
