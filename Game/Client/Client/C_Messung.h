#pragma once

extern fstream file;
extern char Messung;
extern LONGLONG g_Frequency, g_CurentCount, g_LastCount;

void messung_init();
void messung_start();
void messung_stop();
void messung_auswerten();
