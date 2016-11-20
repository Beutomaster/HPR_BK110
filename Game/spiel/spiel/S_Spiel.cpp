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
char gespielte_karten[56];
char gk_zaehler = 0;
char kartenanzahl[4];
char offene_karte[4];
char erste_karte[4];
char letzte_karte[4];
char Spieleranzahl = 0;
char aktiver_spieler = 0;
char spielerstatus[4];
// globale Variablen ende

void Spiel_INIT(int sa) {	//Karten verteilen, Spielfeld verschicken
	srand(unsigned(time(0)));			// Zufallszahl initialisieren
	Spieleranzahl = sa;

	// Spieler aktivieren
	for (int i = 0; i < 4; i++) {
		if (i < Spieleranzahl) spielerstatus[i] = 1;
		else spielerstatus[i] = 0;
	}

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

	// Test: Ausgabe der Spielerkarten
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
	// Test: erste Karten aufdecken
/*	for (int i = 0; i < Spieleranzahl; i++) {
		offene_karte[i] = spielerkarten[i][erste_karte[i]];
		gespielte_karten[gk_zaehler++] = offene_karte[i];
		kartenanzahl[i]--;
	}
*/	
	// Spielfeld verschicken
	broadcast(Spieleranzahl, kartenanzahl, offene_karte, 0);
}

void aktualisieren(char Spieler, char Taste) { //Taste = 1 (Karte aufdecken), = 2 (Klingeln)
	// verarbeitet empfangen Tastendruck
	// aktualisert Spielstatus
	// S_Verbindung - broadcast() aufrufen
	int sp = ((int)Spieler) - 1, ta = (int)Taste, nachricht = 0;

	// nicht auf Spieler ohne Karten warten
	while (kartenanzahl[aktiver_spieler] == 0) aktiver_spieler = (aktiver_spieler + 1) % Spieleranzahl;
	switch (ta) {
	case 1:		// Tastendruck: aufdecken
		if (sp != aktiver_spieler) break;	// Spieler, die nicht dran sind, ignorieren
		else {
			offene_karte[sp] = spielerkarten[sp][erste_karte[sp]];	// erste Karte aufdecken
			erste_karte[sp] = (erste_karte[sp] + 1) % 56;			// erste Karte eins weiter "Ringarray"
			gespielte_karten[gk_zaehler++] = offene_karte[sp];		// offene Karte auf gespielte Kartenhaufen
			kartenanzahl[sp]--;
			aktiver_spieler = (aktiver_spieler + 1) % Spieleranzahl;
		}
		break;
	case 2:		// Tastendruck: klingeln
		if (spielerstatus[sp] == 0) break;		// inaktive Spieler ignorieren

		int farbe[4], wert, klingelflag = 0;	// Kartenfarbe, Kartenwert, Flag für "klingeln ok"
		for (int i = 0; i < Spieleranzahl; i++) {
			farbe[i] = (int)(offene_karte[i] / 10);
		}
		for (int i = 0; i < 4; i++) {			// Auswertung der 4 Kartenfarben
			wert = 0;
			for (int j = 0; j < Spieleranzahl; j++) {		// Auswertung der offenen Karten
				if (i + 1 == farbe[j]) {
					wert = wert + (offene_karte[j] % 10);
				}
			}
			if (wert == 5) {	// klingeln ok -> Spieler erhält alle gespielten Karten
				for (int k = 0; k < gk_zaehler; k++) {
					spielerkarten[sp][letzte_karte[sp]+1] = gespielte_karten[k];
					letzte_karte[sp] = (letzte_karte[sp] + 1) % 56;
					kartenanzahl[sp]++;
				}
				gk_zaehler = 0;
				for (int k = 0; k < Spieleranzahl; k++) {
					offene_karte[k] = NULL;		// offene Karten zurückgesetzt
					if (kartenanzahl[k] == 0) {
						spielerstatus[k] = 0;	//Spieler mit 0 Karten inaktiv
					}
				}
				aktiver_spieler = sp;
				klingelflag = 1;
				break;
			}
		}
		// klingeln falsch -> alle anderen aktiven Spieler erhalten eine Karte vom Klingelden
		if (klingelflag == 0) {
			for (int i = 0; i < Spieleranzahl; i++) {
				if (i != sp && kartenanzahl[sp] > 0 && spielerstatus[i] == 1) {
					spielerkarten[i][letzte_karte[i] + 1] = spielerkarten[sp][erste_karte[sp]];
					letzte_karte[i] = (letzte_karte[i] + 1) % 56;
					kartenanzahl[i]++;
					erste_karte[sp] = (erste_karte[sp] + 1) % 56;
					kartenanzahl[sp]--;
				}
			}
			if (kartenanzahl[sp] == 0) spielerstatus[sp] = 0;	//Spieler mit 0 Karten inaktiv

		}

		// Spielende?
		int spzaehler = Spieleranzahl, gewinner = 0;
		for (int k = 0; k < Spieleranzahl; k++) {
			if (kartenanzahl[k] == 0) spzaehler--;
			else gewinner = k + 1;
		}
		if (spzaehler == 1) {
			cout << "Spieler " << gewinner << " hat gewonnen";
			return;
		}
		klingelflag = 0;
		break;
	}


	nachricht = nachricht + aktiver_spieler;
	//Test
/*	system("CLS");
	for (int i = 0; i < Spieleranzahl; i++) {
		for (int j = erste_karte[i]; j <= letzte_karte[i]; j++) {
			cout << (int)spielerkarten[i][j] << " | ";
		}
		cout << endl;
	}*/

	broadcast(Spieleranzahl, kartenanzahl, offene_karte, nachricht);
} 