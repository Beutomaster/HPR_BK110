#include "C_Verbindung.h"
#include "C_Spiel.h"
#include "S_Verbindung.h"

void c_Verbindung_INIT() {
	//Eingabe IP-Adresse, Warten auf klingeln
}

void c_senden(char Taste) {
	// Tastendruck senden
	// Taste = 1 (Karte aufdecken), = 2 (Klingeln)
	empfangen(Taste);
}

void c_empfangen(char Spieleranzahl, char *Kartenanzahl, char *offene_Karte, char Nachricht) {
	// nimmt Spielfeld entgegen
	// ruft C_Spiel - aktualisieren() auf
	// C_Spiel - Tastendruck() informieren: sperre aufheben
	c_aktualisieren(Spieleranzahl, Kartenanzahl, offene_Karte, Nachricht);
	c_tastendruck();
}