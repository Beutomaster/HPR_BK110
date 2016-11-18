#include "C_Verbindung.h"
#include "C_Spiel.h"
#include <iostream>
#include <Windows.h>	// f�r Konsolenausgabe
#include <conio.h>		// f�r getch()
using namespace std;

void aktualisieren(char Spieleranzahl, char *Kartenanzahl, char *aktuelle_Karte, char Gewinner) {
	// verarbeitet empfangenes Spielfeld
	switch ((int)Spieleranzahl) {
	case 4:
		gotoxy(25, 15);
		cout << "Spieler 4: " << (int)Kartenanzahl[3];
		gotoxy(45, 15);
		cout << (int)offene_Karte[3];
	case 3:
		gotoxy(45, 22);
		cout << "Spieler 3: " << (int)Kartenanzahl[2];
		gotoxy(50, 17);
		cout << (int)offene_Karte[2];
	case 2:
		gotoxy(70, 15);
		cout << "Spieler 2: " << (int)Kartenanzahl[1];
		gotoxy(55, 15);
		cout << (int)offene_Karte[1];
	default:
		gotoxy(45, 8);
		cout << "Spieler 1: " << (int)Kartenanzahl[0];
		gotoxy(50, 13);
		cout << (int)offene_Karte[0];
	}

	gotoxy(0, 30);
} 

void tastendruck() {
	// Taste = 1 (Karte aufdecken), = 2 (Klingeln)
	// Karte aufdecken erlaubt? Spieler dran?
	// l�st C_Senden aus
	// Nach Tastendruck sperrung bis n�chster Broadcast
	char KeyInfo = 0;
	while (42) {
		KeyInfo = _getch();
		switch (KeyInfo) {
		case 'w':
		case 'd':
		case 's':
		case 'a':
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