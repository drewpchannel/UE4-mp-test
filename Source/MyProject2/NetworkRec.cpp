// Fill out your copyright notice in the Description page of Project Settings.
#include <iostream>
#include <stdio.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include "EngineUtils.h"
#include <string>

using namespace std;

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

#include "NetworkRec.h"

TCHAR GetBuf[1024];

ANetworkRec::ANetworkRec()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ANetworkRec::BeginPlay()
{
	Super::BeginPlay();
	RunPrimeTask();
}

void ANetworkRec::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GetActors();

}

void ANetworkRec::RunPrimeTask()
{
	(new FAutoDeleteAsyncTask<NewPrimeSearchTask>())->StartBackgroundTask();
}

//============

NewPrimeSearchTask::NewPrimeSearchTask()
{
}
//destructor used for manual memory cleaning for things outside ue4 scope
//gets called by FAutoDeleteAsync
NewPrimeSearchTask::~NewPrimeSearchTask()
{
	UE_LOG(LogTemp, Warning, TEXT("Task finished"));
}

void NewPrimeSearchTask::DoWork()
{

	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't start Winsock! %i"), wsOk);
		return;
	}

	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

	// Create a server hint structure for the server
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
	serverHint.sin_family = AF_INET; // Address format is IPv4
	//debug to display port for checking on netstat
	u_short PortNumber = 12873; //catch
	int PortNumberInt = (int)PortNumber;
	serverHint.sin_port = htons(PortNumber); // Convert from little to big endian

	// Try and bind the socket to the IP and port
	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAA Can't bind socket! Attempting to close. Error code %i \n"), WSAGetLastError());
		closesocket(in);
		WSACleanup();
		UE_LOG(LogTemp, Warning, TEXT("Tried to close socket %i"), PortNumberInt);
		if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
		{
			UE_LOG(LogTemp, Warning, TEXT("Socket still in use or not opened, wsacleanup might need to rebuild something.... idk aborting"));
			return;
		}
	}

	sockaddr_in client; // Use to hold the client information (port / ip address)
	int clientLength = sizeof(client); // The size of the client information

	char buf[1024];
	for (int p = 10; p > 1; p++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("listening..."));
		ZeroMemory(&client, clientLength); // Clear the client structure
		ZeroMemory(buf, 1024); // Clear the receive buffer

		// Wait for message
		int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR)
		{
			UE_LOG(LogTemp, Warning, TEXT("Error receiving from client %i"), WSAGetLastError());
			continue;
		}

		// Display message and client info
		char clientIp[256]; // Create enough space to convert the address byte array
		ZeroMemory(clientIp, 256); // to string of characters

		// Convert from byte array to chars
		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
		// Display the message / who sent it
		NewPrimeSearchTask::ConvertMessage(buf);
		//UE_LOG(LogTemp, Log, TEXT("%s"), UTF8_TO_TCHAR(buf));
	}
}

void NewPrimeSearchTask::ConvertMessage(char buf[1024])
{
	FVector IncomingVector;
	IncomingVector.InitFromString(buf);
	ApplyToActors(IncomingVector);
}

void NewPrimeSearchTask::ApplyToActors(FVector NewRotation)
{
	TArray<AActor> actors;

	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Itr->ActorHasTag(TEXT("NetworkedPlayer")))
		{
			//UE_LOG(LogTemp, Warning, TEXT("tag found... applying rotation"));
			UE_LOG(LogTemp, Warning, TEXT("original location Z %i"), NewRotation.Z);
			NewRotation.Z += 115.f;
			UE_LOG(LogTemp, Warning, TEXT("new location Z %i"), NewRotation.Z);
			Itr->SetActorLocation(NewRotation);
			FQuat NewDirection = FQuat(NewRotation, 0.f);
			Itr->SetActorRotation(NewDirection, ETeleportType::None);
		}
	}
}