#pragma once
// Platform boundary: keep operating-system-specific code in this file.
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
using socket_t = SOCKET;
constexpr socket_t invalid_socket = INVALID_SOCKET;
inline int closeSocket(socket_t s) { return closesocket(s); }
struct NetRuntime {
    NetRuntime() { WSADATA d{}; if (WSAStartup(MAKEWORD(2,2), &d) != 0) throw std::runtime_error("WSAStartup failed"); }
    ~NetRuntime() { WSACleanup(); }
};
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
using socket_t = int;
constexpr socket_t invalid_socket = -1;
inline int closeSocket(socket_t s) { return close(s); }
struct NetRuntime {};
#endif
#include <stdexcept>
#include <string>