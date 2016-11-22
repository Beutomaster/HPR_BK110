// stdafx.h: Includedatei für Standardsystem-Includedateien
// oder häufig verwendete projektspezifische Includedateien,
// die nur in unregelmäßigen Abständen geändert werden.
//

#pragma once

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <stdio.h>
#include <tchar.h>



// TODO: Hier auf zusätzliche Header, die das Programm erfordert, verweisen.
#include <stdlib.h>
#include <iostream>
#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib,"ws2_32.lib")

// cout und endl-Manipulator aus std Namensraum
using std::cout;
using std::cin;
using std::endl;

extern HWND hWnd;

#define WM_SOCKET (WM_USER + 1)					// private Windows-Nachrichten
#define SERVER_PORT 6000			 				// COM-Server Port, über den der Server erreichbar ist
#define IDD_WSSERVER                    101
#define IDD_WSCLIENT                    101
#define IDC_STATUS                      1000
#define IDC_SENDDATA                    1001
#define IDC_RECEIVE                     1002
#define IDC_SEND                        1003
#define IDC_DESTADDRESS                 1004
#define IDC_CLOSE                       1005
#define IDC_CONNECT                     1006
