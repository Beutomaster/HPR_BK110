// Server.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "S_Verbindung.h"
#include "S_Spiel.h"
#include "S_Messung.h"

//Event Handling for Console Window
bool WINAPI ConsoleHandler(DWORD CEvent)
{
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

int main(){
	//Event-Handler für Consolen-Fenster (zum Aufräumen bei Programmabbruch)
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
	{
		// unable to install handler... 
		// display message to the user
		printf("Unable to install handler!\n");
		return -1;
	}

	Verbindung_INIT();	//TCP-Port öffnen, Aufforderung zum Verbinden, Spielerhandle speichern, Spielstart wenn alle geklingelt haben
	//Spiel_INIT(); //Karten verteilen, Spielfeld verschicken, evtl. der Rest auch
	cout << endl << "Abbruch mit ESC";
	while (42) { // ende wenn Spiel beendet / "esc" auf Server
		// S_Verbindung - empfangen() (fragt Paketeingang ab)
	}


    return 0;
}

