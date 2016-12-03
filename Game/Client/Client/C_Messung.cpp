#include "stdafx.h"
#include "C_Messung.h"

char Messung = 2; // Startklingeln überspringen, da keine Antwort
LONGLONG g_Frequency, g_CurentCount, g_LastCount;

void messung_init() {
	//Frequenz holen
	if (!QueryPerformanceFrequency((LARGE_INTEGER*)&g_Frequency))
		std::cout << "Performance Counter nicht vorhanden" << std::endl;
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
	cout << endl << "Antwortzeit: " << dTimeDiff << " s" << endl;
	Messung = 0;
}