// Client.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "C_Verbindung.h"
#include "C_Spiel.h"
#include "C_Messung.h"

bool WINAPI ConsoleHandler(DWORD CEvent)
{
	char mesg[128];

	switch (CEvent)
	{
	case CTRL_C_EVENT:
		//MessageBox(NULL,"CTRL+C received!", "CEvent", MB_OK);
		break;
	case CTRL_BREAK_EVENT:
		//MessageBox(NULL,"CTRL+BREAK received!", "CEvent", MB_OK);
		break;
	case CTRL_CLOSE_EVENT:
		//MessageBox(NULL,"Program being closed!", "CEvent", MB_OK);
		break;
	case CTRL_LOGOFF_EVENT:
		//MessageBox(NULL,"User is logging off!", "CEvent", MB_OK);
		break;
	case CTRL_SHUTDOWN_EVENT:
		//MessageBox(NULL,"User is logging off!", "CEvent", MB_OK);
		break;

	}
	return TRUE;
}

int main()
{
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
	{
		// unable to install handler... 
		// display message to the user
		printf("Unable to install handler!\n");
		return -1;
	}

	Verbindung_INIT(); //Eingabe IP-Adresse, Warten auf klingeln, evtl. der Rest auch

	while (42) {
		//bei Broadcasts Event -> C_Verbindung empfangen()
		
		//bei Tastendruck Event C_Spiel Tastendruck()
/*		while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
		{
			
			if (bRet == -1)
			{
				// handle the error and possibly exit
			}
			else
			{
				if (FilterMessage(&msg) == 0) // <-- Function you write
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			
		}*/
	}
    return 0;
}

