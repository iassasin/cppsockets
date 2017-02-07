#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include "sockets_share.hpp"

#ifndef OS_WIN
	#include <netinet/in.h>
#else
	#include <winsock.h>
#endif

namespace sockets {

class address {
public:
	virtual string str() const = 0;
	virtual bool valid() const = 0;
};

}

#endif

