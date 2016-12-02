#pragma once

//#include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib,"ws2_32.lib")

extern unsigned char Spieleranzahl;
extern char Spielstart;
extern char Server_on;

void Verbindung_INIT();
void broadcast(unsigned char, unsigned char *, unsigned char *, unsigned char);
void cleanup();
