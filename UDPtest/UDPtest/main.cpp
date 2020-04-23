#include <iostream>
#include <WS2tcpip.h>  //win sockets lib, this is 2.0 Beej: WSACleanup() when you’re all through with the sockets library

#pragma comment (lib, "ws2_32.lib")

using namespace std;  //saves us from using std::

void main() 
{
	//Startup winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wSOk = WSAStartup(version, &data);
	if (wSOk != 0) {
		fprintf(stderr, "WSAStartup failed.\n");
		exit(1);
	} //needs the data behind WSADATA data; info found in docs for ws2_32

	//Bind socket to IP addy \ port
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
	serverHint.sin_family = AF_INET; // Address format is IPv4
	serverHint.sin_port = htons(54100); // Convert from little to big endian

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) //sockaddress (points to struct? & is like obj.serverHint?)
	{
		cout << "can't bind socket " << WSAGetLastError;
		return;
	}

	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);  //replaces everything in memory as 0

	char buf[2048];
	//enter loop, wait for messages
	while (true) {
		ZeroMemory(buf, 2048);  //clears buffer when trying to receive again

		//receive from this socket, to this buffer, this length, flags, take client information and put it in &client
		int bytesIn = recvfrom(in, buf, 2048, 0, (sockaddr*)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR) 
		{
			cout << "Error rec from client";
			continue;
		}
		char clientIP[256];
		ZeroMemory(clientIP, 256);

		//convert number to address
		inet_ntop(AF_INET, &client.sin_addr, clientIP, 256);  //INT family comes from the struct ws2

		cout << "Messaged recieved from: " << clientIP << " : " << buf << endl;
	}

	//close socket
	closesocket(in);

	//shutdown winsock
	WSACleanup();
}