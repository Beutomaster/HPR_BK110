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

unsigned char wtcp_serv();
void read_and_echo(SOCKET);
void new_player(SOCKET);

unsigned char wtcp_serv() //returns Spieleranzahl
{
	unsigned char Spieleranzahl = 0;
	SOCKET MySock, x;
	struct sockaddr_in addr;
	int adresslaenge;
	MSG	   msg;
	char s[20];

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

	cout << endl;

	//Schleife muss noch getrennt werden für Warten auf Spieler und Spiel
	for (;; )
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
				cout << "select()";
				if (x != MySock) {	// data readable on present 
					read_and_echo(x); // nicht fertig
				}
				else {
					new_player(x);	// new connection request
					Spieleranzahl++; // nicht fertig
				}
			}
		}
	}
	return Spieleranzahl;
}

void read_and_echo(SOCKET iClient)	// Read and echo data
{
	int iLen;

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
		sprintf(szBuffer, " %d                 %d", iClient, iLen);
		cout << szBuffer << endl;

		buffer[iLen] = 0;						// mark String end
		cout << buffer << endl;

		cout << "send()" << endl;
		if (send(iClient, (LPSTR)buffer, iLen, 0) < 0)
		{
			in_use[iClient] = 0;
			(void)closesocket(iClient);
			cout << "closesocket()" << endl;
		}
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
	sprintf(szBuffer, "Open Connection from %s - Port %d (Handle %d)",
		remoteIP, htons(peer.sin_port), iClient);
	//ShowStatus(hWnd, szBuffer, ID_LISTING);
	in_use[iClient] = 1;
}

unsigned char Verbindung_INIT() {
	//TCP-Port öffnen, Aufforderung zum Verbinden, Spielerhandle speichern, Spielstart wenn alle geklingelt haben
	unsigned char Spieleranzahl = 0;
	if (wtcp_serv()) {
		cout << "Server gestartet" << endl;
		cout << "Warte auf Spieler" << endl;
	}
	
	return Spieleranzahl; //Spieleranzahl
}

void broadcast(unsigned char Spieleranzahl, unsigned char *Kartenanzahl, unsigned char *aktuelle_Karte, unsigned char Gewinner) {
	// Spielfeld an alle verschicken
	// Spielernummmer senden?
	// Gewinner = 0, solange kein Gewinner - Nach Sieg auf Spielernummer setzen
}

void empfangen() {
	// nimmt Tastendruck entgegen (mit Spielerhandle)
	// ruft S_Spiel - aktualisieren(unsigned char Spieler, unsigned char Taste) bei Paketeingang auf
}