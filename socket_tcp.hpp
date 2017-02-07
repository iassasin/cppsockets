#ifndef SOCKET_TCP_HPP
#define SOCKET_TCP_HPP

#include "address_ip.hpp"
#include "socket.hpp"

namespace sockets {

class socket_tcp_server;

class socket_tcp : public socket {
protected:
	address_ip4 _source, _dest;
	socket_t _socket;
	bool _valid;
	
	socket_tcp(){}
	socket_tcp(socket_t sock, struct sockaddr_in source, struct sockaddr_in dest);
	friend socket_tcp_server;
public:
	socket_tcp(address_ip4 dest);
	virtual ~socket_tcp() override;
	
	virtual size_t recv(void *data, size_t count) override;
	virtual size_t send(const void *data, size_t count) override;
	
	virtual const address &get_source_address() override { return _source; }
	virtual const address &get_dest_address() override { return _dest; }
	
	virtual bool valid() override;
	
	virtual void open();
	virtual void close() override;
};

class socket_tcp_server : public socket_tcp {
protected:

public:
	socket_tcp_server(int port) : socket_tcp_server(address_ip4("0.0.0.0", port)){}
	socket_tcp_server(address_ip4 source);
	
	virtual ~socket_tcp_server() override;
	
	virtual size_t recv(void *data, size_t count) override { return 0; }
	virtual size_t send(const void *data, size_t count) override { return 0; }
	
	virtual bool valid() override;
	
	virtual void open() override { listen(1); }
	
	void listen(int cnt);
	unique_ptr<socket_tcp> accept();
};

}

#endif

