#include "stdafx.h"
#include "C_Messung.h"

#define len_filename 8

fstream file;
char Messung = 2; // Startklingeln überspringen, da keine Antwort
LONGLONG g_Frequency, g_CurentCount, g_LastCount;

void messung_init() {
	//Frequenz holen
	if (!QueryPerformanceFrequency((LARGE_INTEGER*)&g_Frequency))
		std::cout << "Performance Counter nicht vorhanden" << std::endl;
	
	char filename[len_filename + 5] = {"mess0000.txt"};
	filename[len_filename + 4] = '\0';
	
	//if (tmpnam_s(filename, len_filename)) //erzeugen von Zufallsnamen funktioniert nicht
	if (0)
	{
		printf("Error occurred creating unique filename.\n");
		exit(1);
	}
	else
	{
		//filename += ".txt";
		filename[len_filename] = '.';
		filename[len_filename + 1] = 't';
		filename[len_filename + 2] = 'x';
		filename[len_filename + 3] = 't';
		filename[len_filename + 4] = '\0';

		printf("Die Antwortzeiten des Servers auf Tastatur-Eingabe-Nachrichten wird waehrend des Spiels gemessen.\n\n", filename);
		printf("Die Messwerte werden in %s geschrieben.\n\n", filename);
	}

	//Open file
	file.open(filename, ios::out | ios::app);
}

void messung_start() {
	//1. Messung
	QueryPerformanceCounter((LARGE_INTEGER*)&g_CurentCount);
	Messung = 1;
}

void messung_stop() {
	//2. Messung
	QueryPerformanceCounter((LARGE_INTEGER*)&g_LastCount);
}

void messung_auswerten() {
	double dTimeDiff = (((double)(g_LastCount - g_CurentCount)) / ((double)g_Frequency));
	file << dTimeDiff << endl;
	cout << endl << "Antwortzeit: " << dTimeDiff << " s" << endl;
	Messung = 0;
}