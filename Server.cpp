#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"

using namespace std;

int main()
{
	//main locals
	long SUCCESSFUL;
	WSAData WinSockData;
	WORD DLLVERSION; 
	
	//WORDS = objects of a data size that a processor naturally handles (such as 16 or 32 bit)
	DLLVERSION = MAKEWORD(2, 1);//macro to create WORD value by concatenating its arguments

	//Start Winsock DLL
	SUCCESSFUL = WSAStartup(DLLVERSION, &WinSockData);

	//Create Socket Structure
	SOCKADDR_IN ADDRESS;
	int AddressSize = sizeof(ADDRESS);

	//Create Sockets
	SOCKET sock_LISTEN;
	SOCKET sock_CONNECTION;

	int portNum;
	cout << "Enter port number: ";
	cin >> portNum;

	//socket Arguments: AF_INET = internet domain
	//SOCK_STREAM = connection oriented TCP (not SOCK_DGRAM)
	sock_CONNECTION = socket(AF_INET, SOCK_STREAM, NULL);
	ADDRESS.sin_addr.s_addr = inet_addr("192.168.1.86"); //This is my laptop's ip, change this to match your computer's ip
	ADDRESS.sin_family = AF_INET;
	ADDRESS.sin_port = htons(portNum); //port

	sock_LISTEN = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sock_LISTEN, (SOCKADDR*)&ADDRESS, sizeof(ADDRESS));
	listen(sock_LISTEN, SOMAXCONN);

	while (true)
	{
		cout << "SERVER: Waiting for incoming connection...";

		if (sock_CONNECTION = accept(sock_LISTEN, (SOCKADDR*)&ADDRESS, &AddressSize))
		{
			cout << "A connection was found!" << endl;

			char msg[256] = "Welcome! You have connected to the file server!";
			send(sock_CONNECTION, msg, sizeof(msg), NULL);
			closesocket(sock_CONNECTION);
		}
	}

	system("Pause");
	return 0;
}
