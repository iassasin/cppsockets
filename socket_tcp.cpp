#ifndef OS_WIN
	#include <unistd.h>
#endif

#include "socket_tcp.hpp"

namespace sockets {

socket_tcp::socket_tcp(socket_t sock, struct sockaddr_in source, struct sockaddr_in dest){
	_socket = sock;
	_source = address_ip4(source);
	_dest = address_ip4(dest);
	_valid = true;
}

socket_tcp::socket_tcp(address_ip4 dest){
	_socket = SOCKET_ERROR;
	_dest = dest;
	_valid = false;
}

socket_tcp::~socket_tcp(){
	close();
}

size_t socket_tcp::recv(void *data, size_t count){
	if (valid()){
		size_t l = 0;
		while (l < count){
			int k = ::recv(_socket, (void *)((char *) data + l), count - l, 0);
			if (k <= 0){
				_valid = false;
				return l;
			}
			l += k;
		}
		return l;
	}
	return 0;
}

size_t socket_tcp::send(const void *data, size_t count){
	if (valid()){
		size_t l = 0;
		while (l < count){
			int k = ::send(_socket, (const void *)((const char *) data + l), count - l, 0);
			if (k <= 0){
				_valid = false;
				return l;
			}
			l += k;
		}
		return l;
	}
	return 0;
}

bool socket_tcp::valid(){
	return _source.valid() && _dest.valid() && _socket != SOCKET_ERROR && _valid;
}

void socket_tcp::open(){
	if (_dest.valid()){
		_socket = ::socket(AF_INET, SOCK_STREAM, 0);
		if (_socket == SOCKET_ERROR){
			return;
		}
		
		auto addr = _dest.get();
		socklen_t len = sizeof(addr);
		if (::connect(_socket, (struct sockaddr *) &addr, len) == SOCKET_ERROR){
		    close();
		    return;
		}
		
		if (::getsockname(_socket, (struct sockaddr *) &addr, &len) == SOCKET_ERROR || len != sizeof(addr)){
		    close();
		    return;
		}
		
		_source = addr;
		_valid = true;
	}
}

void socket_tcp::close(){
	if (_socket != SOCKET_ERROR){
#ifdef OS_WIN
		::closesocket(_socket);
#else
		::close(_socket);
#endif
		_socket = SOCKET_ERROR;
		_valid = false;
	}
}

// --- socket_tcp_server ---

socket_tcp_server::socket_tcp_server(address_ip4 source)
		: socket_tcp()
{
	_source = source;
}

socket_tcp_server::~socket_tcp_server(){
	
}

bool socket_tcp_server::valid(){
	return _source.valid() && _socket != SOCKET_ERROR && _valid;
}

void socket_tcp_server::listen(int cnt){
	if (_source.valid()){
		_socket = ::socket(AF_INET, SOCK_STREAM, 0);
		if (_socket == SOCKET_ERROR){
			return;
		}

		int val = 1;
		if (::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == SOCKET_ERROR){
			close();
			return;
		}
		
		auto addr = _source.get();
		if (::bind(_socket, (struct sockaddr *) &addr, sizeof(addr)) == SOCKET_ERROR){
			close();
			return;
		}
	
		::listen(_socket, cnt);
		_valid = true;
	}
}

unique_ptr<socket_tcp> socket_tcp_server::accept(){
	if (valid()){
		struct sockaddr_in addr;
		socklen_t addrlen = sizeof(addr);
		socket_t sock = ::accept(_socket, (struct sockaddr *) &addr, &addrlen);
		socket_tcp *s = new socket_tcp(sock, addr, _source.get());
		return unique_ptr<socket_tcp>(s);
	}
	return unique_ptr<socket_tcp>(nullptr);
}

}

