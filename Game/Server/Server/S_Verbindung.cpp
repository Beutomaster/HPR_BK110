#include "S_Verbindung.h"
#include "S_Spiel.h"

int Verbindung_INIT() {
	//TCP-Port öffnen, Aufforderung zum Verbinden, Spielerhandle speichern, Spielstart wenn alle geklingelt haben

	return 0; //Spieleranzahl
}

void broadcast(char Spieleranzahl, char *Kartenanzahl, char *aktuelle_Karte, char Gewinner) {
	// Spielfeld an alle verschicken
	// Spielernummmer senden?
	// Gewinner = 0, solange kein Gewinner - Nach Sieg auf Spielernummer setzen
}

void empfangen() {
	// nimmt Tastendruck entgegen (mit Spielerhandle)
	// ruft S_Spiel - aktualisieren() auf
}