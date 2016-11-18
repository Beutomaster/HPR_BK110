#include "S_Verbindung.h"
#include "S_Spiel.h"
#include "C_Verbindung.h"

int Verbindung_INIT() {
	//TCP-Port öffnen, Aufforderung zum Verbinden, Spielerhandle speichern, Spielstart wenn alle geklingelt haben

	return 0; //Spieleranzahl
}

void broadcast(char Spieleranzahl, char *Kartenanzahl, char *offene_Karte, char Gewinner) {
	// Spielfeld an alle verschicken
	// Spielernummmer senden?
	// Gewinner = 0, solange kein Gewinner - Nach Sieg auf Spielernummer setzen
	c_empfangen(Spieleranzahl, Kartenanzahl, offene_Karte, Gewinner);
}

void empfangen(char Taste) {
	// nimmt Tastendruck entgegen (mit Spielerhandle)
	// ruft S_Spiel - aktualisieren() auf
	char spieler;
	switch (Taste) {
	case 't':
		aktualisieren(1, 1);
		break;
	case 'z':
		aktualisieren(1, 2);
		break;
	case 'h':
		aktualisieren(2, 1);
		break;
	case 'j':
		aktualisieren(2, 2);
		break;
	case 'v':
		aktualisieren(3, 1);
		break;
	case 'b':
		aktualisieren(3, 2);
		break;
	case 'f':
		aktualisieren(4, 1);
		break;
	case 'g':
		aktualisieren(4, 2);
		break;
	case 'q':
		return;
	}
}