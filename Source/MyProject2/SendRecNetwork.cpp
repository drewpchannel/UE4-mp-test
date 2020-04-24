// Fill out your copyright notice in the Description page of Project Settings.

#include <iostream>
#include <WS2tcpip.h>  //win sockets lib, this is 2.0 Beej: WSACleanup() when you’re all through with the sockets library
#include <Windows.h>
#include <string>
#include "Containers/UnrealString.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"

#pragma comment (lib, "ws2_32.lib")
#include "SendRecNetwork.h"

// Sets default values for this component's properties
USendRecNetwork::USendRecNetwork()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USendRecNetwork::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void USendRecNetwork::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SendData();
}

void USendRecNetwork::SendData()
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
	//char buf[1024];
	FVector MyCharacter = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	const TCHAR* message = *MyCharacter.ToString();
	char* people = TCHAR_TO_UTF8(message);

	int sendOK = sendto(out, people, strlen(people), 0, (sockaddr*)&server, sizeof(server));

	if (sendOK == SOCKET_ERROR)
	{
		//cout << "Send didn't work: " << WSAGetLastError;
	}

	closesocket(out);

	WSACleanup();
}