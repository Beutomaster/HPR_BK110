#include "stdafx.h"
#include "C_Verbindung.h"
#include "C_Spiel.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6000"

//Polling Client
//An event based Client with hidden window would be better (create a hidden window by calling the CreateWindowEx method with the dwExStyle parameter set to 0)
int client(PCSTR Server_IP)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
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

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

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

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

void Verbindung_INIT() {
	std::string zeichenkette;

	printf("Bitte Server-Adresse eingeben:\n");

	//Eingabe IP-Adresse
	std::getline(std::cin, zeichenkette); //IP-Notation oder Hostname, Eingabe pr�fen?
	PCSTR Server_IP = (PCSTR) &zeichenkette;

	client(Server_IP);

	//Warten auf klingeln

}

void senden(unsigned char Taste) {
	// Tastendruck senden
	// Taste = 1 (Karte aufdecken), = 2 (Klingeln)
}

void empfangen() {
	// nimmt Spielfeld entgegen
	// ruft C_Spiel - aktualisieren() auf
	// C_Spiel - Tastendruck() informieren: sperre aufheben
}