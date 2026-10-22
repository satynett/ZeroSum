#pragma once
#include "ui/Platform.h"
#include <string>
#include <stdexcept>
class NetworkError : public std::runtime_error { public: using std::runtime_error::runtime_error; };
class Socket {
public:
    Socket() noexcept = default; explicit Socket(socket_t fd) noexcept : fd_(fd) {}
    ~Socket(); Socket(const Socket&) = delete; Socket& operator=(const Socket&) = delete;
    Socket(Socket&& other) noexcept; Socket& operator=(Socket&& other) noexcept;
    static Socket tcp(); void setReuseAddr(); void bind(unsigned short port,const std::string& address="0.0.0.0");
    void listen(int backlog=8); Socket accept(); void connect(const std::string& address,unsigned short port);
    void sendLine(const std::string& line); bool recvLine(std::string& line); void setRecvTimeoutMs(int ms); void close() noexcept;
    bool valid() const noexcept{return fd_!=invalid_socket;} socket_t native() const noexcept{return fd_;}
private: socket_t fd_=invalid_socket; std::string buffer_;
};