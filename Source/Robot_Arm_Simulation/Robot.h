// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Robot.generated.h"

class ABrain;
enum RotationAxis;
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
		UStaticMeshComponent* m_Base;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		FVector m_Base_Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UStaticMeshComponent* m_Link_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UStaticMeshComponent* m_Link_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UStaticMeshComponent* m_Link_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UStaticMeshComponent* m_End_Effector; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UStaticMeshComponent* m_End_Effector_Tip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		FVector m_End_Effector_Offset;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float t0 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float t1 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float t2 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float t3 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float t4 = 0;


private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		ABrain* m_Brain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		FVector m_Brain_Local_Location = {40, 0, 20};

	TMap<FString, int> m_Debug;
public:
	ABrain* GetBrain();
	void SetBrain(ABrain* brain);

	void PlaceBrain();

	FMatrix GetPositionMatrix(FVector position);

	UFUNCTION(BlueprintCallable)
	void PoseRobot(FMatrix translation);
	
	FMatrix RotationMatrix(RotationAxis axis, float theta);
};
