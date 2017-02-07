#include "address_ip.hpp"

#ifndef OS_WIN
	#include <netdb.h>
#endif

namespace sockets {

in_addr_t address_ip4::read_address(string hostname){
	struct hostent *h = gethostbyname(hostname.c_str());
	if (!h){
		return 0;
	}
	return *((unsigned long *) h->h_addr_list[0]);
}

address_ip4::address_ip4(string host, int port){
	struct hostent *h = gethostbyname(host.c_str());
	_valid = h != nullptr;
	if (_valid){
		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(port);
		_addr.sin_addr.s_addr = *((unsigned long *) h->h_addr_list[0]); //htonl(INADDR_ANY);
	}
}

address_ip4::address_ip4(struct sockaddr_in addr){
	_addr = addr;
	_valid = true;
}

address_ip4::address_ip4(const address_ip4 &addr){
	_addr = addr._addr;
	_valid = addr._valid;
}

string address_ip4::str() const {
	stringstream res;
	res << ip() << ":" << port();
	return res.str();
}

bool address_ip4::valid() const {
	return _valid;
}

string address_ip4::ip() const {
	stringstream res;
	in_addr_t a = ntohl(_addr.sin_addr.s_addr);
	for (int i = 0; i < 4; ++i, a <<= 8){
		res << ((a & 0xff000000) >> 24);
		if (i < 3){
			res << ".";
		}
	}
	return res.str();
}

int address_ip4::port() const {
	return ntohs(_addr.sin_port);
}

}

