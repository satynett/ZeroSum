#include "net/InviteCode.h"
#include <cassert>
#include <stdexcept>
int main(){assert(encodeInviteCode("192.168.1.15")=="C0A8010F");assert(decodeInviteCode("C0A8010F")=="192.168.1.15");bool threw=false;try{(void)decodeInviteCode("bad");}catch(const std::invalid_argument&){threw=true;}assert(threw);}