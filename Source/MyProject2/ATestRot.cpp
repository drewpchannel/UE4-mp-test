// Fill out your copyright notice in the Description page of Project Settings.

#include "ATestRot.h"
// might not need GS
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"

AATestRot::AATestRot()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Change this script to a tagging component for network players
void AATestRot::BeginPlay()
{
	Super::BeginPlay();
	this->Tags.Add("NetworkedPlayer");
}

// Called every frame
void AATestRot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	FRotator NewRotation = FRotator(PitchValue, YawValue, RollValue);
	FQuat QuatRotation = FQuat(NewRotation);
	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
	*/
}

