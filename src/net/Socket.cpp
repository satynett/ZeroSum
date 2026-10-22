#include "Socket.h"
#include <cerrno>
#include <utility>
#ifdef _WIN32
#define SOCK_ERR WSAGetLastError()
#else
#define SOCK_ERR errno
#endif
Socket::~Socket(){close();}
Socket::Socket(Socket&& o) noexcept:fd_(o.fd_),buffer_(std::move(o.buffer_)){o.fd_=invalid_socket;}
Socket& Socket::operator=(Socket&& o) noexcept{if(this!=&o){close();fd_=o.fd_;buffer_=std::move(o.buffer_);o.fd_=invalid_socket;}return *this;}
Socket Socket::tcp(){auto s=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);if(s==invalid_socket)throw NetworkError("socket failed "+std::to_string(SOCK_ERR));return Socket(s);}
void Socket::setReuseAddr(){int yes=1;if(setsockopt(fd_,SOL_SOCKET,SO_REUSEADDR,reinterpret_cast<const char*>(&yes),sizeof(yes))<0)throw NetworkError("setsockopt failed");}
void Socket::bind(unsigned short port,const std::string& ip){sockaddr_in a{};a.sin_family=AF_INET;a.sin_port=htons(port);if(inet_pton(AF_INET,ip.c_str(),&a.sin_addr)!=1)throw NetworkError("invalid IPv4");if(::bind(fd_,reinterpret_cast<sockaddr*>(&a),sizeof(a))<0)throw NetworkError("bind failed "+std::to_string(SOCK_ERR));}
void Socket::listen(int n){if(::listen(fd_,n)<0)throw NetworkError("listen failed");}
Socket Socket::accept(){sockaddr_in a{};
#ifdef _WIN32
int n=sizeof(a);
#else
socklen_t n=sizeof(a);
#endif
auto s=::accept(fd_,reinterpret_cast<sockaddr*>(&a),&n);if(s==invalid_socket)throw NetworkError("accept failed");return Socket(s);}
void Socket::connect(const std::string& ip,unsigned short port){sockaddr_in a{};a.sin_family=AF_INET;a.sin_port=htons(port);if(inet_pton(AF_INET,ip.c_str(),&a.sin_addr)!=1)throw NetworkError("invalid IPv4");if(::connect(fd_,reinterpret_cast<sockaddr*>(&a),sizeof(a))<0)throw NetworkError("connect failed");}
void Socket::sendLine(const std::string& line){std::string d=line;if(d.empty()||d.back()!='\n')d+='\n';std::size_t i=0;while(i<d.size()){
#ifdef _WIN32
int n=::send(fd_,d.data()+i,static_cast<int>(d.size()-i),0);
#else
ssize_t n=::send(fd_,d.data()+i,d.size()-i,0);
#endif
if(n<=0)throw NetworkError("send failed");i+=static_cast<std::size_t>(n);}}
bool Socket::recvLine(std::string& line){for(;;){auto p=buffer_.find('\n');if(p!=std::string::npos){line=buffer_.substr(0,p);if(!line.empty()&&line.back()=='\r')line.pop_back();buffer_.erase(0,p+1);if(line.size()>1024)throw NetworkError("line exceeds 1 KiB");return true;}if(buffer_.size()>1024)throw NetworkError("line exceeds 1 KiB");char b[512];
#ifdef _WIN32
int n=::recv(fd_,b,sizeof(b),0);
#else
ssize_t n=::recv(fd_,b,sizeof(b),0);
#endif
if(n==0)return false;if(n<0)throw NetworkError("recv failed");buffer_.append(b,static_cast<std::size_t>(n));}}
void Socket::setRecvTimeoutMs(int ms){
#ifdef _WIN32
DWORD t=static_cast<DWORD>(ms);if(setsockopt(fd_,SOL_SOCKET,SO_RCVTIMEO,reinterpret_cast<const char*>(&t),sizeof(t))<0)throw NetworkError("timeout failed");
#else
timeval t{ms/1000,(ms%1000)*1000};if(setsockopt(fd_,SOL_SOCKET,SO_RCVTIMEO,&t,sizeof(t))<0)throw NetworkError("timeout failed");
#endif
}
void Socket::close() noexcept{if(valid()){closeSocket(fd_);fd_=invalid_socket;}}