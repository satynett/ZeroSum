#pragma once
#include <string>
std::string encodeInviteCode(const std::string& ipv4);
std::string decodeInviteCode(const std::string& code);
std::string getLocalIPv4();