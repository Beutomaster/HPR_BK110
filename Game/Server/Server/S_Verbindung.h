#pragma once

//#include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib,"ws2_32.lib")

unsigned char Verbindung_INIT();
void broadcast(unsigned char, unsigned char *, unsigned char *, unsigned char);
void empfangen();
