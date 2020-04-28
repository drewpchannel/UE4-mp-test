// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimeCalculator.h"

APrimeCalculator::APrimeCalculator()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APrimeCalculator::BeginPlay()
{
	Super::BeginPlay();
	RunPrimeTask(50000);
}

void APrimeCalculator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrimeCalculator::RunPrimeTask(int32 NumPrimes)
{
	(new FAutoDeleteAsyncTask<PrimeSearchTask>(NumPrimes))->StartBackgroundTask();
}

//broken example to freeze game because it is run on the games thread
void APrimeCalculator::RunPrimeTaskOnMain(int32 NumPrimes)
{
	PrimeSearchTask* task = new PrimeSearchTask(NumPrimes);

	task->DoWorkMain();

	delete task;
}

//============

PrimeSearchTask::PrimeSearchTask(int32 _primeCount)
{
	PrimesCount = _primeCount;
}
//destructor used for manual memory cleaning for things outside ue4 scope
//gets called by FAutoDeleteAsync
PrimeSearchTask::~PrimeSearchTask()
{
	UE_LOG(LogTemp, Warning, TEXT("Task finished"));
}

void PrimeSearchTask::DoWork() 
{
	int primesFound = 0;
	int CurrentTestNumber = 2;

	while (primesFound < PrimesCount)
	{
		bool isPrime = true;
		for (int i = 2; i < CurrentTestNumber / 2; i++)
		{
			if (CurrentTestNumber % i == 0)
			{
				isPrime = false;
				break;
			}
		}
		if (isPrime)
		{
			primesFound++;
			
			if (primesFound % 1000 == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Primes Found: %i"), primesFound);
			}
		}
		CurrentTestNumber++;
	}
}

void PrimeSearchTask::DoWorkMain()
{
	DoWork();
}