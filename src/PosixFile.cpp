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


#include "PosixFile.h"

#include <limits>
#include <cassert>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

using namespace std;


namespace dce {


void PosixFile::checkIfOpen() const throw(std::runtime_error) {
   if (!isOpen()) throw runtime_error("File is not open.");
}


void PosixFile::setFileFlag(int flag, bool value) throw(std::runtime_error, std::runtime_error) {
   checkIfOpen();

   int res = ::fcntl(fd(), F_GETFL);
   if (res < 0) throw runtime_error("Getting file flags failed.");
   int flags = res;

   if (value) res = ::fcntl(fd(), F_SETFL, flags | flag);
   else res = ::fcntl(fd(), F_SETFL, flags & (std::numeric_limits<int>::max() - flag));
   if (res < 0) throw runtime_error("Setting file flags failed.");
}


void PosixFile::open(int fd, bool closeFD) throw(std::invalid_argument, std::runtime_error) {
   if (isOpen()) throw runtime_error("Can't open File/socket which is already open.");
   if ((fd<0)) throw invalid_argument("Invalid file descriptor.");
   m_fd = fd; m_closeFD = closeFD;
}


void PosixFile::close() throw(std::runtime_error) {
   checkIfOpen();

   if (isOpen()) {
      if (m_closeFD) ::close(m_fd);
      m_fd = -1;
   }
}


void PosixFile::blocking(bool block) throw(std::runtime_error, std::runtime_error)
   { setFileFlag(O_NONBLOCK, !block); }



bool PosixFile::pollRead(int timeout) throw(std::runtime_error, std::runtime_error) {
   checkIfOpen();
   
   struct pollfd pollFds[1] = {{fd(), POLLIN|POLLPRI, 0}};
#ifdef POLLRDHUP
   pollFds[0].events |= POLLRDHUP;
#endif

   int ret = ::poll(pollFds, 1, timeout);
   if (ret < 0) throw runtime_error("IO Error while polling.");
   if ( (pollFds[0].revents!=0) && !(pollFds[0].revents & (POLLIN|POLLPRI)) ) {
      throw runtime_error("IO Error while polling.");
   }
   return (ret > 0);
}


bool PosixFile::pollWrite(int timeout) throw(std::runtime_error, std::runtime_error) {
   checkIfOpen();
   
   struct pollfd pollFds[1] = {{fd(), POLLOUT|POLLERR|POLLHUP|POLLNVAL, 0}};

   int ret = ::poll(pollFds, 1, timeout);
   if (ret < 0) throw runtime_error("IO Error while polling.");
   if ( (pollFds[0].revents!=0) && !(pollFds[0].revents & POLLOUT) ) {
      throw runtime_error("IO Error while polling.");
   }
   return (ret > 0);
}


size_t PosixFile::read(void *buffer, size_t bufferSize) throw(std::runtime_error, std::runtime_error) {
   checkIfOpen();

   assert( bufferSize <= size_t(std::numeric_limits<ssize_t>::max()) );
   assert( isOpen() );

   ssize_t rc = ::read(m_fd, buffer, bufferSize);
   if (rc < 0) throw runtime_error("Reading from file/socket failed.");
   return size_t(rc);
}


size_t PosixFile::write(const void *buffer, size_t bufferSize) throw(std::runtime_error, std::runtime_error) {
   checkIfOpen();

   assert( bufferSize <= size_t(std::numeric_limits<ssize_t>::max()) );
   assert( isOpen() );

   ssize_t rc = ::write(m_fd, buffer, bufferSize);
   if (rc < 0) throw runtime_error("Writing to file/socket failed.");
   return size_t(rc);
}


void PosixFile::sync() throw(std::runtime_error, std::runtime_error) {
   checkIfOpen();
   
   int rc = ::fsync(m_fd);
   if (rc != 0) throw runtime_error("Sync on file/socket failed.");
}


PosixFile::PosixFile() : m_fd(-1), m_closeFD(true) {}

PosixFile::PosixFile(int fd, bool closeFD) throw(std::invalid_argument, std::runtime_error)
   : m_fd(-1) { open(fd, closeFD); }


PosixFile::~PosixFile() {
   if (isOpen() && m_closeFD) ::close(m_fd);
}




void FSPosixFile::open(const std::string &fileName, AccessMode accMode, CreateMode creaMode)
   throw(std::invalid_argument, std::runtime_error) { open(fileName, accMode, creaMode, false, false); }

   
void FSPosixFile::open(const std::string &fileName, AccessMode accMode, CreateMode creaMode,
   bool nonblock, bool async) throw(std::invalid_argument, std::runtime_error)
{
   if (isOpen()) throw invalid_argument("Can't open File/socket which is already open.");

   int flags = O_NOCTTY;
#ifdef   O_LARGEFILE
   flags |= O_LARGEFILE;
#endif

   switch (accMode) {
      case ACC_READ: flags |= O_RDONLY; break;
      case ACC_WRITE: flags |= O_WRONLY; break;
      case ACC_RDWR: flags |= O_RDWR; break;
      case ACC_APPEND: flags |= O_WRONLY | O_APPEND; break;
      default: throw invalid_argument("Invalid file access mode.");
   }

   switch (creaMode) {
      case CREA_NONE: break;
      case CREA_CREATE: flags |= O_CREAT; break;
      case CREA_TRUNC: flags |= O_CREAT | O_TRUNC; break;
      case CREA_EXCL: flags |= O_CREAT | O_EXCL; break;
      default: throw invalid_argument("Invalid file creation mode.");
   }

   if (nonblock) flags |= O_NONBLOCK;
   if (async) flags |= O_ASYNC;
   
   int newFD = ::open(fileName.c_str(), flags);
   if (newFD < 0) throw runtime_error("Cannot open file '"+fileName+"'.");
   else { m_fd = newFD; m_closeFD = true; }
}


FSPosixFile::FSPosixFile() {}

FSPosixFile::FSPosixFile(const std::string &fileName, AccessMode accMode, CreateMode creaMode, bool nonblock, bool async) throw(std::invalid_argument, std::runtime_error)
   { open(fileName, accMode, creaMode, nonblock, async); }


FSPosixFile::~FSPosixFile() {}


} // namespace dce
