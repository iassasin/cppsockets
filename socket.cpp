#include "socket.hpp"

namespace sockets {

#ifdef OS_WIN
	static WSADATA w_wsa_data;
#endif

bool init_sockets(){
#ifdef OS_WIN
	WSAStartup(MAKEWORD(2, 2), &w_wsa_data);
#endif
	return true;
}

bool release_sockets(){
#ifdef OS_WIN
	WSACleanup();
#endif
	return true;
}

// streambuf_socket //

streambuf_socket::streambuf_socket(socket &sock){
	_sock = &sock;
	_buf = EOF;
}

int streambuf_socket::sync(){
#ifdef _DEBUG_
		LOG << __FUNCTION__ << endl;
#endif
		return 0;
}

streamsize streambuf_socket::showmanyc(){
#ifdef _DEBUG_
	LOG << __FUNCTION__ << endl;
#endif
	return _buf != EOF ? 1 : 0;
}

streamsize streambuf_socket::xsgetn(char *s, streamsize n){
#ifdef _DEBUG_
	LOG << __FUNCTION__ << endl;
#endif
	streamsize sz = _sock->recv(s, n);
	_buf = EOF;
	return sz;
}

int streambuf_socket::_uflow(){
#ifdef _DEBUG_
	LOG << __FUNCTION__ << endl;
#endif
	char c;
	if (_sock->recv(&c, sizeof(c)) == sizeof(c)){
		_buf = c;
	} else {
		_buf = EOF;
	}
	
	return _buf;
}

int streambuf_socket::underflow(){
#ifdef _DEBUG_
	LOG << __FUNCTION__ << endl;
#endif
	if (_buf == EOF){
		return _uflow();
	}
	return _buf;
}

int streambuf_socket::uflow(){
#ifdef _DEBUG_
		LOG << __FUNCTION__ << endl;
#endif
	int c = _buf;
	if (c == EOF){
		c = _uflow();
		if (c == EOF){
			return EOF;
		}
	}
	
	_buf = EOF;
	
	return c;
}

int streambuf_socket::pbackfail(int c){
#ifdef _DEBUG_
	LOG << __FUNCTION__ << endl;
#endif
	return EOF;
}

streamsize streambuf_socket::xsputn(const char *s, streamsize n){
#ifdef _DEBUG_
	LOG << __FUNCTION__ << endl;
#endif
	return _sock->send(s, n);
}

int streambuf_socket::overflow(int c){
#ifdef _DEBUG_
	LOG << __FUNCTION__ << endl;
#endif
	if (_sock->send(c) == sizeof(c)){
		return c;
	}
	return EOF;
}

}

