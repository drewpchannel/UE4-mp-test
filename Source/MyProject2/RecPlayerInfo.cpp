#include "RecPlayerInfo.h"
#include <iostream>
#include "CoreMinimal.h"
#include <WS2tcpip.h>  //win sockets lib, this is 2.0 Beej: WSACleanup() when you’re all through with the sockets library

#pragma comment (lib, "ws2_32.lib")

//find way to check if socket is open in the function
bool IsSocketClosed = true;
SOCKET in;

void URecPlayerInfo::StartDataStream()
{
	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);  //replaces everything in memory as 0
	char buf[2048];
	ZeroMemory(buf, 2048);  //clears buffer when trying to receive again

	int bytesIn = recvfrom(in, buf, 2048, 0, (sockaddr*)&client, &clientLength);
	if (bytesIn == SOCKET_ERROR)
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket error client, rec \n"));
	}
	char ClientIP[256];
	ZeroMemory(ClientIP, 256);
	inet_ntop(AF_INET, &client.sin_addr, ClientIP, 256);
}

URecPlayerInfo::URecPlayerInfo()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URecPlayerInfo::BeginPlay()
{
	Super::BeginPlay();
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wSOk = WSAStartup(version, &data);
	in = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in serverHint;

	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
	serverHint.sin_family = AF_INET; // Address format is IPv4
	serverHint.sin_port = htons(51343); // Convert from little to big endian

	if (IsSocketClosed)
	{
		int BindErrors = bind(in, (sockaddr*)&serverHint, sizeof(serverHint));
		if (BindErrors == 0) //sockaddress (points to struct? & is like obj.serverHint?)
		{
			UE_LOG(LogTemp, Warning, TEXT("failed to socket"));
		}
		else
		{
			if (IsSocketClosed)
			{
				UE_LOG(LogTemp, Warning, TEXT("socket open"));
				IsSocketClosed = false;
			}
		}
	}
	else
	{
		sockaddr_in client;
		int clientLength = sizeof(client);
		ZeroMemory(&client, clientLength);  //replaces everything in memory as 0
		char buf[2048];
		ZeroMemory(buf, 2048);  //clears buffer when trying to receive again

		int bytesIn = recvfrom(in, buf, 2048, 0, (sockaddr*)&client, &clientLength);
		UE_LOG(LogTemp, Warning, TEXT("buf legnth: %d"), strlen(buf));
	}

}

void URecPlayerInfo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//may need freeaddr function to clear up this mem
	StartDataStream();
}