// stdafx.h: Includedatei für Standardsystem-Includedateien
// oder häufig verwendete projektspezifische Includedateien,
// die nur in unregelmäßigen Abständen geändert werden.
//

#pragma once

#include "targetver.h"

#ifndef UNICODE
#define UNICODE
#endif 

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <stdio.h>
#include <tchar.h>

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

// TODO: Hier auf zusätzliche Header, die das Programm erfordert, verweisen.
#include <stdlib.h>
#include <iostream>
#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <conio.h>	// für getch()

#pragma comment(lib,"ws2_32.lib")

using namespace std;

#define WM_SOCKET (WM_USER + 1)					// private Windows-Nachrichten
#define SERVER_PORT 6000			 				// COM-Server Port, über den der Server erreichbar ist
#define KEY_A (WM_USER + 2)
#define KEY_SPACE (WM_USER + 3)
#define KEY_Q (WM_USER + 4)

extern char Client_on;
