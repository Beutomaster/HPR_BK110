// spiel.cpp : Definiert den Einstiegspunkt f�r die Konsolenanwendung.
//

#include "S_Verbindung.h"
#include "S_Spiel.h"



int main()
{
	int Spielerzahl;
	Spielerzahl = Verbindung_INIT();	//TCP-Port �ffnen, Aufforderung zum Verbinden, Spielerhandle speichern, Spielstart wenn alle geklingelt haben
	Spielerzahl = 4;
	Spiel_INIT(Spielerzahl); //Karten verteilen, Spielfeld verschicken


	while (42) { // ende wenn Spiel beendet / "esc" auf Server
				 // S_Verbindung - empfangen() bei Paketeingang
	}


}

