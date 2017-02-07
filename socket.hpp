#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "sockets_share.hpp"
#include "address.hpp"

#ifdef _DEBUG_
#include <iostream>
using namespace std;
#define LOG cerr << ">>> "
#endif

namespace sockets {

using std::streambuf;
using std::streamsize;
using std::iostream;

class socket;

class streambuf_socket : virtual public streambuf {
private:
	int _buf;
	
	streambuf_socket(){};
	
	int _uflow();
protected:
	socket *_sock;
	
	virtual int sync() override;
	
	virtual streamsize showmanyc() override;
	
	virtual streamsize xsgetn (char *s, streamsize n) override;
	virtual int underflow() override;
	virtual int uflow() override;
	virtual int pbackfail (int c) override;
	
	virtual streamsize xsputn (const char *s, streamsize n) override;
	virtual int overflow (int c) override;
public:
	streambuf_socket(socket &sock);
	virtual ~streambuf_socket(){}
};

class socket : public iostream {
protected:
	streambuf_socket _streambuf;
public:
	socket() : iostream(&_streambuf), _streambuf(*this){}
	virtual ~socket(){}
	
	virtual size_t recv(void *data, size_t count) = 0;
	virtual size_t send(const void *data, size_t count) = 0;
	
	virtual const address &get_source_address() = 0;
	virtual const address &get_dest_address() = 0;
	
	virtual bool valid() = 0;
	
	virtual void close() = 0;
	
	template<typename T>
	size_t recv(T &data){ return recv(&data, sizeof(T)); }
	template<typename T>
	size_t send(const T &data){ return send(&data, sizeof(T)); }
};

bool init_sockets();
bool release_sockets();

}

#endif

