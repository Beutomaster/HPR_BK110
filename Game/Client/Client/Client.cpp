// Client.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "C_Verbindung.h"
#include "C_Spiel.h"
#include "C_Messung.h"

//Event Handling for Hidden Window
LRESULT CALLBACK WSClientProc(HWND hWnd, UINT msg, WPARAM wP, LPARAM lP)
{
	switch (msg)
	{

	case WM_SOCKET:							// WINSOCK-Messages
	{
		switch (WSAGETSELECTEVENT(lP))
		{
		case FD_CONNECT:				// Nachricht von connect()
			cout << "FD_CONNECT: ";
			if (WSAGETSELECTERROR(lP) == 0)
			{
				cout << "erfolgreich!" << endl;
				glob_tastensperre = 0;
			}
			else
			{
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				cout << "Kein Server!" << endl;
			}
			break;

		case FD_READ:					// Daten eingegangen
			empfangen();
			break;

		case FD_CLOSE:					// Server hat Verbindung beendet
			cout << "FD_CLOSE" << endl;
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			glob_tastensperre = 1;
			break;
		}
	}
	break;

	/*
	case WM_COMMAND:						// Schaltflächen-Nachrichten
		switch (LOWORD(wP))
		{
		case IDCANCEL:                  // CloseBox des Fensters
			EndDialog(hWnd, 0);
			break;


		case IDC_CONNECT:
		{ SOCKADDR_IN sin;
		char remoteIP[64];
		char Buffer[80];
		// Zieladresse auslesen
		GetWindowText(GetDlgItem(hWnd, IDC_DESTADDRESS),remoteIP,sizeof(remoteIP));

		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(SERVER_PORT);
		// IP-Adresse -> Dot Notation
		cout << "inet_addr()" << endl;
		sin.sin_addr.s_addr = inet_addr(remoteIP);

		// Adresse über DNS auflösen
		if (sin.sin_addr.s_addr == INADDR_NONE)
		{
			HOSTENT *he;
			cout << "gethostbyname()" << endl;
			he = gethostbyname(remoteIP);
			if (he)
				sin.sin_addr.s_addr = *((DWORD*)he->h_addr);
			else
			{
				cout << "ungülitige Internet-Adresse" << endl;
				break;
			}
		}
		// Zieladresse protokollieren
		wsprintf(Buffer, "Adresse: 0x%08lx",
			ntohl(sin.sin_addr.s_addr));
		cout << Buffer << endl;
		// Socket erzeugen
		cout << "socket()" << endl;
		ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);

		if (ConnectSocket == INVALID_SOCKET)
		{
			cout << "Fehler beim Allokieren des Connect-Sockets" << endl;
			cout << "Kein Verbindungsaufbau möglich" << endl;
			break;
		}

		// asynchronen Mode aktivieren
		cout << "WSAAsyncSelect()" << endl;
		if (WSAAsyncSelect(ConnectSocket,
			hWnd,
			WM_SOCKET,
			FD_CONNECT |
			FD_READ |
			FD_CLOSE) == 0)
		{
			cout << "connect()" << endl;
			if (connect(ConnectSocket, (SOCKADDR*)&sin, sizeof(sin))
				== SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					cout << "Warte..." << endl;
					// Senden-Schalfläche deaktivieren
					//EnableWindow(GetDlgItem(hWnd, IDC_SEND),FALSE);
					break;
				}
			}
		}
		else
			cout << "Fehler bei WSAAsyncSelect()" << endl;

		cout << "closesocket()" << endl;
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		}
		break;
		}
		break;
		*/

	}
	return DefWindowProc(hWnd, msg, wP, lP);
	//return FALSE;
}

//Event Handling for Console Window
bool WINAPI ConsoleHandler(DWORD CEvent)
{
	char mesg[128];

	switch (CEvent)
	{
	case CTRL_C_EVENT:
		//MessageBox(NULL,"CTRL+C received!", "CEvent", MB_OK);
		cleanup();
		break;
	case CTRL_BREAK_EVENT:
		//MessageBox(NULL,"CTRL+BREAK received!", "CEvent", MB_OK);
		cleanup();
		break;
	case CTRL_CLOSE_EVENT:
		//MessageBox(NULL,"Program being closed!", "CEvent", MB_OK);
		cleanup();
		break;
	case CTRL_LOGOFF_EVENT:
		//MessageBox(NULL,"User is logging off!", "CEvent", MB_OK);
		cleanup();
		break;
	case CTRL_SHUTDOWN_EVENT:
		//MessageBox(NULL,"User is logging off!", "CEvent", MB_OK);
		cleanup();
		break;
	}
	return TRUE;
}

int main()
{
	//Event-Handler für Consolen-Fenster (zum Aufräumen bei Programmabbruch)
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
	{
		// unable to install handler... 
		// display message to the user
		printf("Unable to install handler!\n");
		return -1;
	}

	//Hidden Window erzeugen, um Socket-Events an Fenster zu leiten
	const wchar_t class_name[] = _T("HiddenWindow");
	WNDCLASSEX wx = {};
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc = (WNDPROC) WSClientProc; // pWndProc;        // function which will handle messages
	wx.hInstance = GetModuleHandle(NULL); // current_instance;
	wx.lpszClassName = class_name;

	if (RegisterClassEx(&wx)) {
		hWnd = CreateWindowEx(0, wx.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, wx.hInstance, NULL);
		//cout << GetLastError() << endl;
		if (hWnd == NULL) cout << "Hidden Message-Window could not be created!\n";
	} else cout << "Could not create HiddenWindow class!\n";

	Verbindung_INIT(); //Eingabe IP-Adresse, Warten auf klingeln, evtl. der Rest auch
	//cout << "Verbindung_INIT() ended" << endl;

	/*
	// Filter Main Message Queque for Socket-Events (maybe an alternative to a hidden Window)
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {

			if (bRet == -1) {
				// handle the error and possibly exit
			}
			else {
				if (FilterMessage(&msg) == 0) { // <-- Function you write
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
	}
	*/

	//Waiting for Events
	while (42) {
		//bei Broadcasts Event -> C_Verbindung empfangen()
		
		//bei Tastendruck Event C_Spiel Tastendruck()
		tastendruck();

	}
    return 0;
}

