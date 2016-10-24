// Client.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"

#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<WinSock2.h>
#include<iostream>
using namespace std;
SOCKET Connection;


void ClientThread() {
	char buffer[256];
	while (true) {
		recv(Connection, buffer, sizeof(buffer), NULL);
		std::cout << buffer << std::endl;
	}
}
int main() {
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock starup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	SOCKADDR_IN addr;//Address to be binded to our Connection socket
	int addrlen = sizeof(addr);//Need sizeofaddr for the connect function
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);//Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {//If we are unable to connect
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	std::cout << "Connected...!!" << std::endl;
	//char MOTD[256];
	//recv(Connection, MOTD, sizeof(MOTD), NULL);//Receive Message of the Day bufferin to MOTD array
	//std::cout << "MOTD: " << MOTD << std::endl << endl;

	char sendToSer[256];
	char getServer[256];

	while (true) {
		recv(Connection, getServer, sizeof(getServer), NULL);
		cout << getServer << endl;
		fflush(stdin);
		cin >> sendToSer;
		send(Connection, sendToSer, sizeof(sendToSer), NULL);
		
	}

	system("pause");
	return 0;
}