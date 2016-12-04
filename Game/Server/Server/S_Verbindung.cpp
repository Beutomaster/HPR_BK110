#include "stdafx.h"
#include "S_Verbindung.h"
#include "S_Spiel.h"

#define	IN_PORT	6000
#define	MAX_SOCKETS	500
#define	BUFFER_SIZE	512							// maximal COM-Server Packet length

char    buffer[BUFFER_SIZE];
int     in_use[MAX_SOCKETS];					// Client Handles
struct	timeval  seltime;
fd_set  readfds;

char    szBuffer[256];						// used for messages
char FAR* remoteIP;

char wtcp_serv();
void empfangen(SOCKET, unsigned char);
void new_player(SOCKET);
char Spielstart = 0;
char Server_on = 1;

struct Spielteilnehmer {
	SOCKET Socket[MAX_PLAYER];
	unsigned char Start[MAX_PLAYER];
} Player;


SOCKET MySock, x;

char wtcp_serv() //returns 0 on error, 1 if normally closed
{
	struct sockaddr_in addr;
	int adresslaenge;
	//MSG	   msg;
	char s[20];
	WSADATA wsaData;
	int iResult;
	unsigned char Spieler = 0;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 0;
	}

	for (x = 0; x < MAX_SOCKETS; x++)
		in_use[x] = 0;
	// open socket, protocol TCP
	cout << "socket()";
	if ((MySock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << " failed.";
		return 0;
	}

	cout << endl;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(IN_PORT);
	addr.sin_addr.s_addr = 0;

	adresslaenge = sizeof(addr);
	getsockname(MySock, (struct sockaddr *) &addr, &adresslaenge);
	_itoa(addr.sin_addr.s_addr, s, 10);
	cout << "Adresse : " << s << " " << endl;
	// bind socket to server port and IP-address
	cout << "bind()";
	if (bind(MySock, (const struct sockaddr FAR *)&addr, sizeof(addr)) < 0)
	{
		(void)closesocket(MySock);
		cout << " failed.";
		return 0;
	}

	cout << endl;

	adresslaenge = sizeof(addr);
	getsockname(MySock, (struct sockaddr *) &addr, &adresslaenge);
	_itoa(addr.sin_addr.s_addr, s, 10);
	cout << "Adresse : " << s << " " << endl;

	// listening for incoming connections 
	cout << "listen()";
	if (listen(MySock, 5) < 0)
	{
		(void)closesocket(MySock);
		cout << " failed.";
		return 0;
	}

	cout << endl << endl;

	cout << "Server gestartet" << endl;
	cout << "Warte auf Spieler" << endl << endl;

	//Empfangsschleife
	while (Server_on)
	{
		FD_ZERO((fd_set FAR *)&readfds);		// clear fd_count (struct fd_set)
		FD_SET(MySock, (fd_set FAR *)&readfds);
		seltime.tv_sec = 0;
		seltime.tv_usec = 100;					// maximum time for select to wait

		for (x = 0; x < MAX_SOCKETS; x++)		// set fd_set for all sockets
		{
			if (in_use[x])
				FD_SET(x, (fd_set FAR *)&readfds);
		}



		if (!select(MAX_SOCKETS,
			(fd_set FAR *)&readfds, (fd_set FAR *) 0, (fd_set FAR *) 0,
			(const struct timeval FAR *)&seltime))
		{
			//Solution for closesocket without Window-Messages needed!!!
			/*
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					for (x = 0; x < MAX_SOCKETS; x++)	// close all aktiv Clients
					{
						if (in_use[x] && x != MySock)
							(void)closesocket(x);
					}
					(void)closesocket(MySock);
					return;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			*/
			continue;
		}

		for (x = 0; x < MAX_SOCKETS; x++)
		{
			if (FD_ISSET(x, (fd_set FAR *)&readfds))
			{
				cout << "select()" << endl;
				if (x != MySock) {	// data readable on present
					for (Spieler = 0; Spieler < MAX_PLAYER; Spieler++) {
						if(x==Player.Socket[Spieler-1]) break;
					}
					cout << "Incomming Message from Player: " << (int) Spieler << endl;
					empfangen(x, Spieler);
				}
				else {
					if (Spieleranzahl <= MAX_PLAYER && !Spielstart ) {
						new_player(x);	// new connection request
					}
				}
			}
		}
	}
	return 1;
}

void Verbindung_INIT() {
	//TCP-Port öffnen, Aufforderung zum Verbinden, Spielerhandle speichern, Spielstart wenn alle geklingelt haben
	Spieleranzahl = 0;
	for (char i = 0; i < MAX_PLAYER; i++) {
		Player.Start[i]=0;
	}
	while (!wtcp_serv()) {
	}
}

void new_player(SOCKET MySock)	// accept new connection
{
	SOCKET  iClient;
	struct	sockaddr_in peer;
	int		peersize = sizeof(peer);

	cout << "accept()" << endl;
	if ((iClient = accept(MySock,
		(struct sockaddr FAR *)&peer, (LPINT)&peersize)) < 0)
	{
		cout << "WARNING: Active connections: accept" << endl;
		return;
	}

	remoteIP = inet_ntoa(peer.sin_addr);		// IP-address -> Dot-Notation
	sprintf(szBuffer, "Open Connection from %s - Port %d (Handle %d)", remoteIP, htons(peer.sin_port), iClient);
	cout << szBuffer << endl;
	in_use[iClient] = 1;
	Player.Socket[Spieleranzahl] = iClient;
	Spieleranzahl++;
	cout << "Spieleranzahl: " << (int)Spieleranzahl << endl;
	//Spielernummer senden
	char iLen = 1;
	buffer[0] = Spieleranzahl;
	cout << "send() to " << iClient << endl;
	if (send(iClient, (LPSTR)buffer, iLen, 0) < 0)
	{
		in_use[iClient] = 0;
		(void)closesocket(iClient);
		cout << "closesocket()" << endl;
	}
}

void empfangen(SOCKET iClient, unsigned char Spieler)	// Read and send update broadcast
														// nimmt Tastendruck entgegen (mit Spielerhandle)
														// ruft S_Spiel - aktualisieren(unsigned char Spieler, unsigned char Taste) bei Paketeingang auf
{
	int iLen;
	unsigned char Taste = 0;

	cout << "recv()" << endl;
	if ((iLen = recv(iClient, (LPSTR)buffer, sizeof(buffer), 0)) <= 0)
	{
		cout << "failed" << endl;
		in_use[iClient] = 0;
		(void)closesocket(iClient);
		sprintf(szBuffer, "closesocket(%d)", iClient);
		cout << szBuffer << endl;
	}
	else
	{
		sprintf(szBuffer, "Handle: %d, Bytes: %d, ", iClient, iLen);
		cout << szBuffer;

		buffer[iLen] = 0;						// mark String end
		cout << "Message:";
		for (int i = 0; i < iLen; i++) {
			cout << " " << (int)buffer[i];
		}
		cout << endl;

		if (iLen != 2) {
			printf("Fehlerhafte Nachricht!\n");
		}
		else {
			Taste = buffer[0];
			if (Spielstart) {
				if (buffer[1] == spielzug) aktualisieren(Spieler, Taste);
			}
			else {
				if (Taste == 2) {
					Player.Start[Spieler - 1] = 1;
					if (Spieleranzahl > 1) {
						Spielstart = 1;
						for (int i = 0; i < Spieleranzahl; i++) {
							Spielstart = Spielstart && Player.Start[i];
						}
					}
					if (Spielstart) {
						cout << "Spielstart\n";
						Spiel_INIT();
					}
				}
			}
		}

		/*
		//echo
		cout << "send()" << endl;
		if (send(iClient, (LPSTR)buffer, iLen, 0) < 0)
		{
		in_use[iClient] = 0;
		(void)closesocket(iClient);
		cout << "closesocket()" << endl;
		}
		*/
	}
}

void broadcast(unsigned char Spieleranzahl, unsigned char *Kartenanzahl, unsigned char *aktuelle_Karte, unsigned char Nachricht) {
	// Spielfeld an alle verschicken
	// Spielernummmer senden???
	// Gewinner = 0, solange kein Gewinner - Nach Sieg auf Spielernummer setzen

	SOCKET iClient;

	//Nachricht im buffer erstellen
	char iLen = 3 + Spieleranzahl * 2;
	buffer[0] = Spieleranzahl;
	for (int i = 0; i < Spieleranzahl; i++) {
		buffer[1 + i] = Kartenanzahl[i];
		buffer[1 + Spieleranzahl + i] = aktuelle_Karte[i];
	}
	buffer[1 + Spieleranzahl * 2] = Nachricht;
	buffer[2 + Spieleranzahl * 2] = spielzug;

	//send
	for (iClient = 0; iClient < MAX_SOCKETS; iClient++) {
		if (in_use[iClient]) {
			cout << "send() to " << iClient << endl;
			if (send(iClient, (LPSTR)buffer, iLen, 0) < 0)
			{
				in_use[iClient] = 0;
				(void)closesocket(iClient);
				cout << "closesocket()" << endl;
			}
		}	
	}	
}

void cleanup() {
	for (x = 0; x < MAX_SOCKETS; x++)	// close all aktiv Clients
	{
		if (in_use[x] && x != MySock)
			(void)closesocket(x);
	}
	(void)closesocket(MySock);
	WSACleanup();
	Server_on = 0;
	return;
}