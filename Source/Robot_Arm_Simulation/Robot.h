// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Robot.generated.h"

class ABrain;

UCLASS()
class ROBOT_ARM_SIMULATION_API ARobot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Calculate();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		ABrain* m_Brain;

	TMap<FString, int> m_Debug;
public:
	ABrain* GetBrain();
	void SetBrain(ABrain* brain);

	FMatrix GetPositionMatrix(FVector position);
};
