#include "stdafx.h"
#include "C_Verbindung.h"
#include "C_Spiel.h"
#include <iomanip>  // für setw()
#include <conio.h>	// für getch()

unsigned char glob_tastensperre = 0;

void aktualisieren(unsigned char Spieleranzahl, unsigned char *kartenanzahl, unsigned char *offene_karte, unsigned char Nachricht) {
	// verarbeitet empfangenes Spielfeld
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
	glob_tastensperre = 0;
} 

void tastendruck() {
	// Taste = 1 (Karte aufdecken), = 2 (Klingeln)
	// Karte aufdecken erlaubt? Spieler dran?
	// löst C_Senden aus
	// Nach Tastendruck sperrung bis nächster Broadcast

	unsigned char KeyInfo = 0;
	while (42) { //!glob_tastensperre) {
		KeyInfo = _getch();
		switch (KeyInfo) {
		case 'a': senden('a');//1);	// aufdecken
			glob_tastensperre = 1;
			break;
		case ' ': senden('k');//2);	// klingeln
			glob_tastensperre = 1;
			break;
		case 'q': //verlässt noch nicht das programm
			cleanup();
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