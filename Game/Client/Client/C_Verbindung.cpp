#include "stdafx.h"
#include "C_Verbindung.h"
#include "C_Spiel.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6000"

HWND hWnd = NULL;
char debug=1; //Debug-Message-Switch
SOCKET ConnectSocket = INVALID_SOCKET;				// Über diesen Socket wird die Verbindung zum Server hergestellt

//Polling Client
//An event based Client with hidden window would be better (create a hidden window by calling the CreateWindowEx method with the dwExStyle parameter set to 0)
char client(PCSTR Server_IP)
{
	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	unsigned char Spieleranzahl = 0;
	unsigned char Kartenanzahl[4];
	unsigned char aktuelle_Karte[4];
	unsigned char Nachricht = 0;
	

	// Validate the parameters
	/*
	if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}
	*/

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(Server_IP, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// register Socket in Message Queque from hidden window
		cout << "WSAAsyncSelect()" << endl;
		if (WSAAsyncSelect(ConnectSocket,hWnd,WM_SOCKET,FD_CONNECT|FD_READ|FD_CLOSE) == 0) {
		}
		else {
			cout << "Fehler bei WSAAsyncSelect()" << endl;
			cout << WSAGetLastError() << endl;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) { //connect noch nicht fertig
				cout << "Warte..."; //auf Event FD_CONNECT
				glob_tastensperre = 1;
				// Senden-Schalfläche deaktivieren
				//EnableWindow(GetDlgItem(hWnd, IDC_SEND),FALSE);
			}
			else {
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				continue;
			}
			
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	/*
	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			Spieleranzahl = recvbuf[0];
			if (iResult != (Spieleranzahl * 2 + 2)) {
				printf("Fehlerhafte Nachricht!\n");
			}
			else {
				for (int i = 0; i < Spieleranzahl; i++) {
					Kartenanzahl[i] = recvbuf[1+i];
					aktuelle_Karte[i] = recvbuf[1+Spieleranzahl+i];
				}
				Nachricht = recvbuf[1+Spieleranzahl*2];
				aktualisieren(Spieleranzahl, Kartenanzahl, aktuelle_Karte, Nachricht);
				tastendruck();
			}
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);
	*/

	return 0;
}

void Verbindung_INIT() {
	string zeichenkette;

	printf("Bitte Server-Adresse eingeben:\n");

	//Eingabe IP-Adresse
	getline(std::cin, zeichenkette); //IP-Notation oder Hostname, Eingabe prüfen?
	PCSTR Server_IP = zeichenkette.c_str();

	client(Server_IP);
	
	//Warten auf klingeln

}

void senden(unsigned char Taste) {
	// Tastendruck senden
	// Taste = 1 (Karte aufdecken), = 2 (Klingeln)
	if (ConnectSocket != INVALID_SOCKET)
	{
		char Buffer[255];
		int  iSendLen=1;
		Buffer[0]=Taste;
		cout << "FD_WRITE" << endl;
		// Serveradresse einlesen
		//iSendLen = GetWindowText(GetDlgItem(hWnd, IDC_SENDDATA), Buffer, sizeof(Buffer));
		cout << "send()...";

		if (send(ConnectSocket, Buffer, iSendLen, 0) != SOCKET_ERROR)
		{
			cout << "... fertig" << endl;
			//EnableWindow(GetDlgItem(hWnd, IDC_SEND), TRUE);
		}
		else
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				cout << "... blockiert" << endl;
			else
				cout << "Fehler bei send()" << endl;

			cout << "closesocket()" << endl;
			//EnableWindow(GetDlgItem(hWnd, IDC_SEND), TRUE);
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			cout << "Abbruch" << endl;
		}
	}
}

void empfangen() {
	// nimmt Spielfeld entgegen
	// ruft C_Spiel - aktualisieren() auf
	// C_Spiel - Tastendruck() informieren: sperre aufheben
	unsigned char Spieleranzahl = 0;
	unsigned char Kartenanzahl[4];
	unsigned char aktuelle_Karte[4];
	unsigned char Nachricht = 0;

	char recvbuf[DEFAULT_BUFLEN];
	int  iReadLen;

	cout << "FD_READ" << endl;
	// Alle verfügbaren Daten einlesen
	iReadLen = recv(ConnectSocket, recvbuf, sizeof(recvbuf) - 1, 0);
	if (iReadLen > 0) {
		recvbuf[iReadLen] = 0;
		if (debug) {
			printf("Bytes received: %d\n", iReadLen);
			cout << recvbuf << endl;
		}
		Spieleranzahl = recvbuf[0];
		if (iReadLen != (Spieleranzahl * 2 + 2)) {
			printf("Fehlerhafte Nachricht!\n");
		}
		else {
			for (int i = 0; i < Spieleranzahl; i++) {
				Kartenanzahl[i] = recvbuf[1 + i];
				aktuelle_Karte[i] = recvbuf[1 + Spieleranzahl + i];
			}
			Nachricht = recvbuf[1 + Spieleranzahl * 2];
			aktualisieren(Spieleranzahl, Kartenanzahl, aktuelle_Karte, Nachricht);
		}
	}
}

void cleanup() {
	if (ConnectSocket != INVALID_SOCKET) {	// Socket schließen
		glob_tastensperre = 1;
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		cout << "closesocket()" << endl;
	}
	WSACleanup();
	DestroyWindow(hWnd);
}