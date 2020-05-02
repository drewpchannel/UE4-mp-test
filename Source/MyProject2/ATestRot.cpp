// Fill out your copyright notice in the Description page of Project Settings.

#include "ATestRot.h"
// might not need GS
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"

// Sets default values
AATestRot::AATestRot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PitchValue = 0.f;
	YawValue = 0.f;
	RollValue = 0.f;

}

// Called when the game starts or when spawned
void AATestRot::BeginPlay()
{
	Super::BeginPlay();
	this->Tags.Add("NetworkedPlayer");
}

// Called every frame
void AATestRot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = FRotator(PitchValue, YawValue, RollValue);
	FQuat QuatRotation = FQuat(NewRotation);
	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

