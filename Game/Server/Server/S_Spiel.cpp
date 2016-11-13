#include "S_Verbindung.h"
#include "S_Spiel.h"
#include <iostream>
#include <algorithm>	// für random_shuffle zum Kartenmischen
#include <time.h>		// zur Zufallsinitialisierung
#include <math.h>

using namespace std;

// globale Variablen
char karten[56];
char spielerkarten[4][56];	// [Spieler][Karten]
char kartenanzahl[4];
char offene_karte[4];
char erste_karte[4];
char letzte_karte[4];
char Spieleranzahl = 0;
// globale Variablen ende

void Spiel_INIT(int Spieleranzahl) {	//Karten verteilen, Spielfeld verschicken
	srand(unsigned(time(0)));			// Zufallszahl initialisieren
	Spieleranzahl = sz;

	// Spielerkarten Array initialisieren
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 60; j++) {
			spielerkarten[i][j] = 0;
		}
	}
	
	// Karten erstellen: 4 Farben (10,20,30,40) 5 Werte (1,2,3,4,5)
	{
		int gruppe = 1, wert = 1, zaehler = 0;

		for (int i = 0; i < 56; i++) {
			karten[i] = gruppe * 10 + wert;

			if ((i + 1) % 14 == 0) gruppe++;
			if (zaehler < 2) zaehler++;
			else {
				wert++;
				zaehler = 0;
			}
			if (wert == 5 && zaehler == 2) {
				wert = 1;
				zaehler = 0;
			}
		}
	}
	
	// Karten mischen
	random_shuffle(begin(karten), end(karten));

	// Karten verteilen
	for (int i = 0; i < 56; i++) {
		spielerkarten[i % Spieleranzahl][i / Spieleranzahl] = karten[i];
	}
	{
		int anzahl = ceil(56.0 / Spieleranzahl);
		for (int i = 0; i < Spieleranzahl; i++) {
			kartenanzahl[i] = anzahl;			// Kartenanzahl setzen
			offene_karte[i] = NULL;				// erste Karte setzen
			erste_karte[i] = 0;					// letzte Karte setzen
			letzte_karte[i] = anzahl-1;			// letzte Karte setzen
		}
		if (Spieleranzahl == 3) {
			kartenanzahl[2] = 18;				// Spieler 3 nur 18 Karten
			letzte_karte[2] = 17;
		}
	}

	// Testausgabe der Spielerkarten
/*	cout << "Testausgabe der Spielerkarten:" << endl;
	for (int i = 0; i < Spieleranzahl; i++) {
		cout << "Spieler: " << i + 1 << ", Anzahl Karten: " << static_cast<int>(kartenanzahl[i]) << ", erste Karten: " << static_cast<int>(spielerkarten[i][erste_karte[i]]) << ", letzte Karten: " << static_cast<int>(spielerkarten[i][letzte_karte[i]]) << endl;
		for (int j = 0; j < 56; j++) {
			cout << static_cast<int>(spielerkarten[i][j]) << " | ";
		}
		cout << endl << endl;
	}
	cout << endl;
*/
	// Spielfeld verschicken
	for (int i = 0; i < Spieleranzahl; i++) {
		offene_karte[i] = spielerkarten[i][erste_karte[i]];
	}
	cout << "1: " << (int)offene_karte[0] << " | 2: " << (int)offene_karte[1] << " | 3: " << (int)offene_karte[2] << " | 4: " << (int)offene_karte[3];
	broadcast(Spieleranzahl, kartenanzahl, offene_karte, 0);
}

void aktualisieren(char Spieler, char Taste) { //Taste = 1 (Karte aufdecken), = 2 (Klingeln)
	// verarbeitet empfangen Tastendruck
	// aktualisert Spielstatus
	// S_Verbindung - broadcast() aufrufen
	switch (Taste) {	// Anpassen an Übergabe mit Spielernr
	case 'w':
		offene_karte[0] = spielerkarten[0][++erste_karte[0]];
		kartenanzahl[0]--;
		break;
	case 'd':
		offene_karte[1] = spielerkarten[1][++erste_karte[1]];
		kartenanzahl[1]--;
		break;
	case 's':
		offene_karte[2] = spielerkarten[2][++erste_karte[2]];
		kartenanzahl[2]--;
		break;
	case 'a':
		offene_karte[3] = spielerkarten[3][++erste_karte[3]];
		kartenanzahl[3]--;
		break;
	}
	broadcast(Spieleranzahl, kartenanzahl, offene_karte, 0);
} 