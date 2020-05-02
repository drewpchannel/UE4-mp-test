// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MYPROJECT2_API ConvertAndRotateActors
{
public:
	FRotator ConvertMessage(char buf[1024]);
	void ApplyToActors(FRotator SendResults);
	ConvertAndRotateActors();
	~ConvertAndRotateActors();

private:
	float x;
	float y;
	float z;
};
