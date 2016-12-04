// Server.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "S_Verbindung.h"
#include "S_Spiel.h"

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

int main(){
	cout << "H A L L I G A L L I - S E R V E R" << endl << endl;

	//Event-Handler für Consolen-Fenster (zum Aufräumen bei Programmabbruch)
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
	{
		// unable to install handler... 
		// display message to the user
		printf("Unable to install handler!\n");
		return -1;
	}

	Verbindung_INIT();	//TCP-Port öffnen, Aufforderung zum Verbinden, Spielerhandle speichern
						//Spielstart mit Spiel_INIT(), wenn alle geklingelt haben.
						//Spielfeld aktualisieren und verschicken

    return 0;
}

