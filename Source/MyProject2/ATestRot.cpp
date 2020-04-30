// Fill out your copyright notice in the Description page of Project Settings.


#include "ATestRot.h"

// Sets default values
AATestRot::AATestRot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AATestRot::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(10.0f, 10.0f, 10.0f), ETeleportType::None);
}

// Called every frame
void AATestRot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

