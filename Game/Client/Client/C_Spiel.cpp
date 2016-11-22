#include "C_Verbindung.h"
#include "C_Spiel.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <Windows.h>	// für Konsolenausgabe
#include <conio.h>		// für getch()
using namespace std;

//int glob_tastensperre = 1;

void aktualisieren(char Spieleranzahl, char *kartenanzahl, char *offene_karte, char Nachricht) {
	// empfangenes Spielfeld bearbeiten
	string spieler[4];
	int farbe, wert;
	for (int i = 0; i < Spieleranzahl; i++) {
		spieler[i] = "";
		farbe = (int)(offene_karte[i] / 10);
		wert = (int)(offene_karte[i] % 10);
		for (int j = 0; j < wert; j++) {
			switch (farbe) {
			case 1:
				spieler[i] = spieler[i] + "A ";
				break;
			case 2:
				spieler[i] = spieler[i] + "B ";
				break;
			case 3:
				spieler[i] = spieler[i] + "C ";
				break;
			case 4:
				spieler[i] = spieler[i] + "D ";
				break;
			}
		}
	}
	system("CLS");
	cout << hex << (int)Nachricht << dec;
	// empfangenes Spielfeld ausgeben
	switch ((int)Spieleranzahl) {
	case 4:
		gotoxy(20, 10);
		cout << "Spieler 4: " << setw(2) << (int)kartenanzahl[3];
		gotoxy(40, 10);
		cout << setw(10) << left << spieler[3];
	case 3:
		gotoxy(45, 17);
		cout << "Spieler 3: " << setw(2) << (int)kartenanzahl[2];
		gotoxy(45, 12);
		cout << setw(10) << left << spieler[2];
	case 2:
		gotoxy(70, 10);
		cout << "Spieler 2: " << setw(2) << (int)kartenanzahl[1];
		gotoxy(55, 10);
		cout << setw(10) << left << spieler[1];
	default:
		gotoxy(45, 3);
		cout << "Spieler 1: " << setw(2) << (int)kartenanzahl[0];
		gotoxy(45, 8);
		cout << setw(10) << left << spieler[0];
	}

	gotoxy(0, 20);
	if (Nachricht & 0x80) {
		cout << "Spieler " << (Nachricht & 0x03) + 1 << " hat richtig geklingelt und erhaelt alle gespielten Karten." << endl;
	}
	if (Nachricht & 0x40) {
		cout << "Spieler " << ((Nachricht & 0x0C) / 4) + 1 << " hat falsch geklingelt und gibt allen anderen Spielern eine Karte." << endl;
	}
	if (Nachricht & 0x20) {
		cout << "Spieler " << ((Nachricht & 0x0C) / 4) + 1 << " hat keine Karten mehr und ist ausgeschieden" << endl;
	}
	if (Nachricht & 0x10) {
		cout << "Spieler " << (Nachricht & 0x03) + 1 << " hat gewonnen";
		// ToDo allgemeine Tastensperre?
	}
	else {
		cout << "Spieler " << (Nachricht & 0x03) + 1 << " ist an der Reihe." << endl;
	}
	//	glob_tastensperre = 0;
}


void tastendruck() {
						// Taste = 1 (Karte aufdecken), = 2 (Klingeln)
						// Karte aufdecken erlaubt? Spieler dran?
						// löst C_Senden aus
						// Nach Tastendruck sperrung bis nächster Broadcast
	char KeyInfo = 0;
	//	while (glob_tastensperre) {}
	while (42) {
		KeyInfo = _getch();
		switch (KeyInfo) {
		case 't':		// Spieler 1: aufdecken
		case 'z':		// Spieler 1: klingeln
		case 'h':		// Spieler 2: aufdecken
		case 'j':		// Spieler 2: klingeln
		case 'v':		// Spieler 3: aufdecken
		case 'b':		// Spieler 3: klingeln
		case 'f':		// Spieler 4: aufdecken
		case 'g':		// Spieler 4: klingeln
		//			glob_tastensperre = 1;
			senden(KeyInfo);
			return;
		case 'q':
			return;
		}
	}
}

// Hilfsfunktion zur Konsolenausgabe
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}