// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Head.generated.h"

class ABrain;

UCLASS()
class ROBOT_ARM_SIMULATION_API AHead : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	ABrain* m_Brain;

public:
	ABrain* GetBrain();
	void SetBrain(ABrain* brain);
};
