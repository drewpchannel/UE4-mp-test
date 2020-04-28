// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrimeCalculator.generated.h"

UCLASS()
class MYPROJECT2_API APrimeCalculator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrimeCalculator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void RunPrimeTask();

	UFUNCTION(BlueprintCallable)
	void RunPrimeTaskOnMain();

};

//=========

class PrimeSearchTask : public FNonAbandonableTask
{
public:
	PrimeSearchTask();
	//destructor used for manual memory cleaning for things outside ue4 scope
	//auto call when task ends i guess
	~PrimeSearchTask();

	//req ue4
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(PrimeSearchTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	int32 PrimesCount;

	void DoWork();

	//test to check if non threading slows your game
	void DoWorkMain();
};