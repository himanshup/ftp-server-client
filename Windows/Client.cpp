#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"

using namespace std;

int main()
{
	long SUCCESSFUL;
	WSAData WinSockData;
	WORD DLLVersion;
	DLLVersion = MAKEWORD(2, 1);
	SUCCESSFUL = WSAStartup(DLLVersion, &WinSockData);

	string response;
	string converter;
	char message[200];

	SOCKADDR_IN address;
	
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, NULL);

	string addr;
	cout << "Enter server ip: ";
	cin >> addr;

	int portNum;
	cout << "Enter server port: ";
	cin >> portNum;

	address.sin_addr.s_addr = inet_addr(addr.c_str());
	address.sin_family = AF_INET;
	address.sin_port = htons(portNum);

	if (connect(sock, (SOCKADDR*)&address, sizeof(address)) == 0)
	{
		recv(sock, message, sizeof(message), NULL);
		converter = message;
		cout << converter << endl;
	}
	else
	{
		cout << "Failed to connect to server" << endl;
	}

	do
	{
		cout << ">ftp ";
		cin >> response;
		if (response == "put")
		{

		}
		else if (response == "get")
		{

		}
		else if (response == "ls")
		{

		}
		else if (response == "quit")
		{
			closesocket(sock);
		}
		else
		{
			cout << "The was an invalid command. Please try again." << endl;
		}
	} while (response != "quit");

	cout << "\n";
	system("Pause");
	return 0;
}
