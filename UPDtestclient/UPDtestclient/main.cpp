#include <iostream>
#include <WS2tcpip.h>  //win sockets lib, this is 2.0 Beej: WSACleanup() when you’re all through with the sockets library
#include <string>

#pragma comment (lib, "ws2_32.lib")

using namespace std;  //saves us from using std::

void main(int argc, char* argv[])
{
	//Startup winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wSOk = WSAStartup(version, &data);
	if (wSOk != 0) {
		fprintf(stderr, "WSAStartup failed.\n");
		exit(1);
	} //needs the data behind WSADATA data; info found in docs for ws2_32

	// Hint struct for server
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(54100);

	inet_pton(AF_INET, "192.168.1.172", &server.sin_addr);

	// Socket creation
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);  //look slike dgram sets up UDP
	/*
	if (bind(out, (sockaddr*)&server.sin_addr, sizeof(&server.sin_addr)) == SOCKET_ERROR)
	{
		cout << "Socket error " << WSAGetLastError << endl;
	}
	*/

	// Write out to socket
	string s(argv[1]);
	int sendOK = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

	if (sendOK == SOCKET_ERROR)
	{
		cout << "Send didn't work: " << WSAGetLastError;
	}

	closesocket(out);

	WSACleanup();
}
