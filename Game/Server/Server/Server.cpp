// Server.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "S_Verbindung.h"
#include "S_Spiel.h"
#include "S_Messung.h"


int main(){
	int Spielerzahl;
	Spielerzahl = Verbindung_INIT();	//TCP-Port öffnen, Aufforderung zum Verbinden, Spielerhandle speichern, Spielstart wenn alle geklingelt haben
	Spiel_INIT(Spielerzahl); //Karten verteilen, Spielfeld verschicken, evtl. der Rest auch
	cout << endl << "Abbruch mit ESC";
	while (42) { // ende wenn Spiel beendet / "esc" auf Server
		// S_Verbindung - empfangen() (fragt Paketeingang ab)
	}


    return 0;
}

