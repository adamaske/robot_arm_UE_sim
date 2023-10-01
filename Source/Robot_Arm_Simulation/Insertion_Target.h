// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Insertion_Target.generated.h"

UCLASS()
class ROBOT_ARM_SIMULATION_API AInsertion_Target : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInsertion_Target();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float m_Order = 0;
public:
	
	float GetOrder() {
		return m_Order;
	}

	void SetOrder(float order) {
		m_Order = order;
	}
};
