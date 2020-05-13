// Fill out your copyright notice in the Description page of Project Settings.
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include "EngineUtils.h"
#include "Engine/World.h"
#include <string>

using namespace std;

// Include the Winsock library (lib) file
#pragma comment(lib, "user32.lib")
#pragma comment (lib, "ws2_32.lib")

#include "NetworkRec.h"

#define BUF_SIZE 256
#define PORT 12773

TCHAR szName[] = TEXT("Global\\MyFileMappingObject");
TCHAR szMsg[] = TEXT("Message from first process test2.");

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
	ConvertSharedMem();

}

void ANetworkRec::RunPrimeTask()
{
	(new FAutoDeleteAsyncTask<NewPrimeSearchTask>())->StartBackgroundTask();
}

void ANetworkRec::FindAndRotateNetA(wstring ConvServerStr)
{
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (Actor && Actor->ActorHasTag("NetworkedPlayer"))
		{
			FVector ServerVector;
			string InitString(ConvServerStr.begin(), ConvServerStr.end());
			FString FInitString = UTF8_TO_TCHAR(InitString.c_str());
			ServerVector.InitFromString(FInitString);
			ServerVector.Z += 110.f;
			Actor->SetActorLocation(ServerVector);
		}
	}
}

void ANetworkRec::ConvertSharedMem()
{
	HANDLE hMapFile;
	LPCTSTR pBuf;

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		szName);               // name of mapping object

	if (hMapFile == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("GT Could not open file mapping object (%i).\n"),
			GetLastError());
	}

	pBuf = (LPTSTR)MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("GT Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);
	}

	if (pBuf != NULL)
	{
		wstring ConvServerStr = pBuf;
		FindAndRotateNetA(ConvServerStr);
	}

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
	u_short PortNumber = PORT; //catch
	int PortNumberInt = (int)PortNumber;
	serverHint.sin_port = htons(PortNumber); // Convert from little to big endian

	// Try and bind the socket to the IP and port
	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't bind socket! Attempting to close. Error code %i \n"), WSAGetLastError());
		closesocket(in);
		WSACleanup();
		UE_LOG(LogTemp, Warning, TEXT("Tried to close socket %i"), PortNumberInt);
		if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
		{
			UE_LOG(LogTemp, Warning, TEXT("Socket still in use or not opened... will not connect"));
			return;
		}
	}

	sockaddr_in client; // Use to hold the client information (port / ip address)
	int clientLength = sizeof(client); // The size of the client information

	char buf[1024];
	for (int p = 10; p > 1; p++)
	{
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

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
		NewPrimeSearchTask::WriteSharedMem(buf);
	}
}

void NewPrimeSearchTask::WriteSharedMem(char buf[1024])
{
	{
		HANDLE hMapFile;
		LPCTSTR pBuf;

		hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			BUF_SIZE,                // maximum object size (low-order DWORD)
			szName);                 // name of mapping object

		if (hMapFile == NULL)
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not create file mapping object %i.\n"), GetLastError());
		}
		pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
			FILE_MAP_ALL_ACCESS, // read/write permission
			0,
			0,
			BUF_SIZE);

		if (pBuf == NULL)
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not map view of file %i.\n"), GetLastError());

			CloseHandle(hMapFile);
		}
		TCHAR* ConvBuf = UTF8_TO_TCHAR(buf);
		//UE_LOG(LogTemp, Warning, TEXT("sending %s to mem"), ConvBuf);
		CopyMemory((PVOID)pBuf, ConvBuf, (_tcslen(ConvBuf) * sizeof(TCHAR)));
		_getch();
	}
}