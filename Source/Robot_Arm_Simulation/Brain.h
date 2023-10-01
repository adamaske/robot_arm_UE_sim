// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Brain.generated.h"

UENUM()
enum RotationAxis { X, Y, Z };

class AInsertion_Target;
class UInsertion_Target_Component;
UCLASS()
class ROBOT_ARM_SIMULATION_API ABrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	 
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		USceneComponent* m_Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UInsertion_Target_Component* m_Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UInsertion_Target_Component* m_Entry;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UInsertion_Target_Component* m_X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		FVector m_Target_Local_Location = { 20,0,0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		FVector m_Entry_Local_Location = { 30,0,0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		FVector m_X_Local_Location = { 40,0,0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TEnumAsByte<RotationAxis> m_RotationAxis = Y;

	TMap<TEnumAsByte<RotationAxis>, FMatrix> m_RotationMatrices;
public:
	FVector GetTargetLocalLocation() {
		return m_Target_Local_Location;
	};
	FVector GetEntryLocalLocation() {
		return m_Entry_Local_Location;
	};
	FVector GetXLocalLocation() {
		return m_X_Local_Location;
	};

public:
	FMatrix GetRotationMatrix() {
		m_RotationMatrices.Empty();
		auto t = GetThetaRadians();
		auto ct = FMath::Cos(t);
		auto st = FMath::Sin(t);


		auto x_rot_m = FMatrix(
			FPlane(1, 0, 0, 0),
			FPlane(0, ct, -st, 0),
			FPlane(0, st, ct, 0),
			FPlane(0, 0, 0, 1)
		);
		auto y_rot_m = FMatrix(
			FPlane(ct, 0, st, 0),
			FPlane(0, 1, 0, 0),
			FPlane(-st, 0, ct, 0),
			FPlane(0, 0, 0, 1)
		);

		auto z_rot_m = FMatrix(
			FPlane(ct, -st, 0, 0),
			FPlane(st, ct, 0, 0),
			FPlane(0, 0, 1, 0),
			FPlane(0, 0, 0, 0)
		);

		m_RotationMatrices.Add(X, x_rot_m);
		m_RotationMatrices.Add(Y, y_rot_m);
		m_RotationMatrices.Add(Z, z_rot_m);

		return m_RotationMatrices[m_RotationAxis];
	}
	 
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		float m_Theta_Degrees = 90;
	
public:
	void SetThetaDegrees(float theta) {
		m_Theta_Degrees = theta;
	}
	void SetThetaRadians(float theta) {
		auto degrees = FMath::RadiansToDegrees(theta);
		m_Theta_Degrees = degrees;
	}
	float GetThetaRadians() {
		return FMath::DegreesToRadians(m_Theta_Degrees);
	}
	float GetThetaDegrees() {
		return m_Theta_Degrees;
	}

};
