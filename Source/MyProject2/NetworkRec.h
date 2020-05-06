// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetworkRec.generated.h"

UCLASS()
class MYPROJECT2_API ANetworkRec : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANetworkRec();
	void FindActors(FVector NewRotation);
	void EmptyFunction(FVector idkwhat);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void RunPrimeTask();
};

//=========

class NewPrimeSearchTask : public FNonAbandonableTask
{
public:
	NewPrimeSearchTask();
	//destructor used for manual memory cleaning for things outside ue4 scope
	//auto call when task ends i guess
	~NewPrimeSearchTask();
	void ConvertMessage(char buf[1024]);
	void WriteSharedMem(char buf[1024]);
	//void ApplyToActors(FVector NewRotation, AActor *FoundActors);

	//req ue4
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(NewPrimeSearchTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();
};