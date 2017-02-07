#ifndef SOCKETS_SHARE_HPP
#define SOCKETS_SHARE_HPP

#ifdef OS_WIN
	#include <winsock.h>
#endif

#include <string>
#include <sstream>
#include <memory>

namespace sockets {

#ifdef OS_WIN
	typedef SOCKET socket_t;
	typedef unsigned int in_addr_t;
#else
	typedef int socket_t;
	#define SOCKET_ERROR -1
#endif

using std::string;
using std::stringstream;
using std::size_t;
using std::unique_ptr;

}

#endif

