// Fill out your copyright notice in the Description page of Project Settings.



#include "Containers/UnrealString.h"
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

#include "ServerTickComp.h"

//Startup winsock
WSADATA data;
WORD version = MAKEWORD(2, 2);
int wSOk = WSAStartup(version, &data);

//Bind socket to IP addy \ port
SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
sockaddr_in serverHint;

char clientIP[256];


sockaddr_in client;
int clientLength;  //replaces everything in memory as 0

char buf[1024];
//enter loop, wait for messages

//close socket
//closesocket(in);

//shutdown winsock
//WSACleanup();

// Sets default values for this component's properties
UServerTickComp::UServerTickComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UServerTickComp::BeginPlay()
{
	Super::BeginPlay();
	BindSocket();

	// ...
	
}


// Called every frame
void UServerTickComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ZeroMemory(buf, 1024);  //clears buffer when trying to receive again

	if (buf[0] != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("buf 0 is not 0"));
	}
		ZeroMemory(clientIP, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIP, 256);  //INT family comes from the struct ws2
		//UE_LOG(LogTemp, Warning, TEXT("beep beep beep"));
}

void UServerTickComp::BindSocket()
{
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
	serverHint.sin_family = AF_INET; // Address format is IPv4
	serverHint.sin_port = htons(54000); // Convert from little to big endian

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) //sockaddress (points to struct? & is like obj.serverHint?)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't bind socket"));
		return;
	}

	int clientLenth = sizeof(client);
	ZeroMemory(&client, clientLength);

	int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
	if (bytesIn == SOCKET_ERROR)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error rec from client"));
	}
}