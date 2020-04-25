#include <iostream>
#include "CoreMinimal.h"
#include <WS2tcpip.h>  //win sockets lib, this is 2.0 Beej: WSACleanup() when you’re all through with the sockets library

#pragma comment (lib, "ws2_32.lib")

#include "RecPlayerInfo.h"

//find way to check if socket is open in the function
bool IsSocketClosed = true;

URecPlayerInfo::URecPlayerInfo()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URecPlayerInfo::BeginPlay()
{
	Super::BeginPlay();
	closesocket(GetSocket());

}

void URecPlayerInfo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Startup winsock

	//mayben check 1 by one if null
	//mayeb pull this out and see if it works in just c++
	SOCKET in = GetSocket();
	sockaddr_in serverHint;

	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
	serverHint.sin_family = AF_INET; // Address format is IPv4
	serverHint.sin_port = htons(53900); // Convert from little to big endian

	if (IsSocketClosed)
	{
		if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) //sockaddress (points to struct? & is like obj.serverHint?)
		{
			UE_LOG(LogTemp, Warning, TEXT("failed to socket"));
		}
		else
		{
			//StartDataStream(in);
			IsSocketClosed = false;
			UE_LOG(LogTemp, Warning, TEXT("socket open"));
		}
	}
}

SOCKET URecPlayerInfo::GetSocket()
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wSOk = WSAStartup(version, &data);
	if (wSOk != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WSA error"));
	} //needs the data behind WSADATA data; info found in docs for ws2_32
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

	return in;
}

void URecPlayerInfo::StartDataStream(SOCKET in)
{
	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);  //replaces everything in memory as 0
	char buf[2048];
	ZeroMemory(buf, 2048);  //clears buffer when trying to receive again

	int bytesIn = recvfrom(in, buf, 2048, 0, (sockaddr*)&client, &clientLength);
	if (bytesIn == SOCKET_ERROR)
	{
	}
	char ClientIP[256];
	ZeroMemory(ClientIP, 256);
	inet_ntop(AF_INET, &client.sin_addr, ClientIP, 256);  //INT family comes from the struct ws2

	//check if its a new client, intro message
	//removed check for serverjoin

	FString myString(buf);
	UE_LOG(LogTemp, Warning, TEXT("Information recieved: %s \n"), *myString);
}