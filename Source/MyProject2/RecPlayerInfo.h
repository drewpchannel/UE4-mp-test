// Fill out your copyright notice in the Description page of Project Settings.
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RecPlayerInfo.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT2_API URecPlayerInfo : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URecPlayerInfo();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual SOCKET GetSocket();
	virtual void StartDataStream(SOCKET in);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
