#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib,"ws2_32.lib")

void Verbindung_INIT();
void senden(unsigned char);
void empfangen();
