#include "InviteCode.h"
#include "ui/Platform.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>
std::string encodeInviteCode(const std::string& ip){in_addr a{};if(inet_pton(AF_INET,ip.c_str(),&a)!=1)throw std::invalid_argument("Invalid IPv4 address");std::ostringstream o;o<<std::uppercase<<std::hex<<std::setw(8)<<std::setfill('0')<<ntohl(a.s_addr);return o.str();}
std::string decodeInviteCode(const std::string& code){if(code.size()!=8)throw std::invalid_argument("Invite code must be 8 hex characters");unsigned long n=0;std::istringstream in(code);in>>std::hex>>n;if(!in||!in.eof()||n>0xffffffffUL)throw std::invalid_argument("Invalid invite code");in_addr a{};a.s_addr=htonl(static_cast<unsigned long>(n));char out[INET_ADDRSTRLEN]{};if(!inet_ntop(AF_INET,&a,out,sizeof(out)))throw std::runtime_error("inet_ntop failed");return out;}
std::string getLocalIPv4(){char host[256]{};if(gethostname(host,sizeof(host))!=0)return "127.0.0.1";addrinfo h{};h.ai_family=AF_INET;h.ai_socktype=SOCK_STREAM;addrinfo* res=nullptr;if(getaddrinfo(host,nullptr,&h,&res)!=0)return "127.0.0.1";std::string out="127.0.0.1";for(auto*p=res;p;p=p->ai_next){auto*a=reinterpret_cast<sockaddr_in*>(p->ai_addr);std::string candidate=inet_ntoa(a->sin_addr);if(candidate.rfind("127.",0)!=0){out=candidate;break;}}freeaddrinfo(res);return out;}