// Fill out your copyright notice in the Description page of Project Settings.

#include <iostream>
#include <tchar.h>
#include <string>
#include "EngineUtils.h"
#include "ConvertAndRotateActors.h"

using namespace std;

ConvertAndRotateActors::ConvertAndRotateActors()
{

}

FRotator ConvertAndRotateActors::ConvertMessage(char buf[1024])
{
    FRotator SendResults;
    return SendResults;
}

void ConvertAndRotateActors::ApplyToActors(FRotator SendResults)
{
	TArray<AActor> actors;

	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Itr->ActorHasTag(TEXT("NetworkedPlayer")))
		{
			//UE_LOG(LogTemp, Warning, TEXT("tag found... applying rotation"));
			FRotator NewRotation = SendResults;
			FQuat QuatRotation = FQuat(NewRotation);
			Itr->AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
		}
	}
}

ConvertAndRotateActors::~ConvertAndRotateActors()
{
}
