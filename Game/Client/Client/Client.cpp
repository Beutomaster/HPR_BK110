// Client.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "C_Verbindung.h"
#include "C_Spiel.h"
#include "C_Messung.h"

MSG msg;
//HANDLE ghEvents[2];
char Client_on = 1;

DWORD WINAPI ThreadProc(LPVOID);

//Event Handling for Hidden Window
LRESULT CALLBACK WSClientProc(HWND hWnd, UINT msg, WPARAM wP, LPARAM lP)
{
	switch (msg)
	{
	case KEY_A: senden(1);	// aufdecken
		break;
	case KEY_SPACE: senden(2);	// klingeln
		break;
	case KEY_Q: cleanup();
		break;

	case WM_SOCKET:							// WINSOCK-Messages
	{
		switch (WSAGETSELECTEVENT(lP))
		{
		case FD_CONNECT:				// Nachricht von connect()
			cout << "FD_CONNECT: ";
			if (WSAGETSELECTERROR(lP) == 0)
			{
				cout << "erfolgreich!" << endl << endl;
				cout << "Ziel des Spiels ist es alle Karten zu gewinnen." << endl;
				cout << "Wer keine Karten mehr hat scheidet aus." << endl;
				cout << "Die Spieler muessen reihum die naechste Karte mit der Taste a von ihrem Stapel aufdecken." << endl;
				cout << "Wer mit Leertaste klingelt, wenn insgesamt fuenf Augen von der jeweilgen Farbe A bis D auf dem Tisch liegen, gewinnt alle aufgedeckten Karten." << endl;
				cout << "Wer falsch klingelt, muss an jeden Spieler eine Karte abgeben." << endl << endl;
				cout << "Wenn mindestens zwei Spieler mit dem Server verbunden sind und geklingelt haben, startet das Spiel." << endl << endl;
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
	}
	return DefWindowProc(hWnd, msg, wP, lP);
	//return FALSE;
}

//Event Handling for Console Window
bool WINAPI ConsoleHandler(DWORD CEvent)
{

	switch (CEvent)
	{
	case CTRL_C_EVENT:
		printf("Ctrl-C event\n");
		cleanup();
		break;
	case CTRL_BREAK_EVENT:
		printf("Ctrl-Break event\n");
		cleanup();
		break;
	case CTRL_CLOSE_EVENT:
		printf("Ctrl-Close event\n");
		cleanup();
		break;
	case CTRL_LOGOFF_EVENT:
		printf("Ctrl-Logoff event\n");
		cleanup();
		break;
	case CTRL_SHUTDOWN_EVENT:
		printf("Ctrl-Shutdown event\n");
		cleanup();
		break;
	}
	return TRUE;
}

bool iskeypressed()
{
	return WaitForSingleObject(
		GetStdHandle(STD_INPUT_HANDLE),
		0
	) == WAIT_OBJECT_0;
}

int main()
{
	HANDLE hThread;
	DWORD dwThreadID;

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

	cout << "H A L L I G A L L I" << endl << endl;

	while (Verbindung_INIT()); //Eingabe IP-Adresse, Verbindung aufbauen bis Erfolg

	// Create a thread for Key-Inputs
	hThread = CreateThread(
		NULL,         // default security attributes
		0,            // default stack size
		(LPTHREAD_START_ROUTINE)ThreadProc,
		NULL,         // no thread function arguments
		0,            // default creation flags
		&dwThreadID); // receive thread identifier

	if (hThread == NULL)
	{
		printf("CreateThread error: %d\n", GetLastError());
		return 1;
	}

	//Waiting for Events
	while (Client_on) {
		//bei Broadcasts Event -> C_Verbindung empfangen()
		//bei Tastendruck Event C_Verbindung senden()

		//if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) //lastet den Prozessor voll aus, mit Tastendruck-Events kann man GetMessage benutzen
		if (GetMessage(&msg, hWnd, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//tastendruck(); //prüft ob Taste gedrückt wurde und sendet diese (falls nicht gesperrt)

	}
    return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{

	// lpParam not used in this example
	UNREFERENCED_PARAMETER(lpParam);

	unsigned char KeyInfo = 0;

	while (Client_on) {
		iskeypressed(); //waits until a key is pressed
		KeyInfo = _getch_nolock();
		//cout << "Taste: " << KeyInfo << endl;
		switch (KeyInfo) {
		case 'a':
			if (!glob_tastensperre) {
				glob_tastensperre = 1;
				//senden(1);	// aufdecken
				SendMessage(hWnd, KEY_A, 0, 0);
			}
			break;
		case ' ':
			if (!glob_tastensperre) {
				SendMessage(hWnd, KEY_SPACE, 0, 0);
				//senden(2);	// klingeln
				glob_tastensperre = 1;
			}
			break;
		case 'q': //verlässt das Programm
			//cleanup();
			SendMessage(hWnd, KEY_Q, 0, 0);
		}
	}
	return 0;
}

