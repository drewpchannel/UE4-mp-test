#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include "Engine/World.h"
#include "Containers/UnrealString.h"

#pragma comment (lib, "ws2_32.lib")

#include "SendPlayerInfo.h"

SOCKET out;
sockaddr_in server;

USendPlayerInfo::USendPlayerInfo()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USendPlayerInfo::BeginPlay()
{
	Super::BeginPlay();
	StartSocket();
	const char* JoinServerMsg = "serverjoin";
	WriteToSocket(JoinServerMsg);
	
}

void USendPlayerInfo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//possibly just send the FString somehow in the future?
	FVector MyCharacter = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FString CharVector = MyCharacter.ToString();
	std::string VectorString(TCHAR_TO_UTF8(*CharVector));
	const char* message = VectorString.c_str();
	WriteToSocket(message);

}

void USendPlayerInfo::StartSocket()
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wSOk = WSAStartup(version, &data);
	if (wSOk != 0) {
		fprintf(stderr, "WSAStartup failed.\n");
		exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(12878); //send

	inet_pton(AF_INET, "192.168.1.172", &server.sin_addr);
	out = socket(AF_INET, SOCK_DGRAM, 0);
}

void USendPlayerInfo::WriteToSocket(const char* message)
{
	int sendOK = sendto(out, message, strlen(message), 0, (sockaddr*)&server, sizeof(server));

}