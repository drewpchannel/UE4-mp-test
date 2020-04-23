// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPlayerRotation.h"

// Sets default values
ACheckPlayerRotation::ACheckPlayerRotation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACheckPlayerRotation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckPlayerRotation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

