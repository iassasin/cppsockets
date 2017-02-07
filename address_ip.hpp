#ifndef ADDRESS_IP_HPP
#define ADDRESS_IP_HPP

#include "address.hpp"

namespace sockets {

class address_ip4 : public address {
protected:
	struct sockaddr_in _addr;
	bool _valid;
	
	
	static in_addr_t read_address(string hostname);
public:
	address_ip4(){ _valid = false; }
	
	address_ip4(string host, int port);
	address_ip4(struct sockaddr_in addr);
	address_ip4(const address_ip4 &addr);
	
	virtual string str() const override;
	virtual bool valid() const override;
	
	string ip() const;
	int port() const;
	
	struct sockaddr_in get() const { return _addr; }
};

}

#endif

