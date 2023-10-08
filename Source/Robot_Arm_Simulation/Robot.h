// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Robot.generated.h"

struct DH_param {
	float t = 0;
	float alpha = 0;
	float r = 0;
	float d = 0;
};

UENUM(BlueprintType)
enum EPose_Mode{ANGLE, IK};

USTRUCT(BlueprintType)
struct FRobot_Pose {
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EPose_Mode> m_Mode = EPose_Mode::ANGLE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_t0 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_t1 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_t2 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_t3 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_t4 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMatrix m_Target_Matrix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_Target_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_Target_Orientation_X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_Target_Orientation_Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_Target_Orientation_Z;
};

USTRUCT(BlueprintType)
struct FRobot_Pose_Report {
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_Actual_Location = { 0,0,0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_DH_FK_Location = { 0,0,0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_Error = 0;
};

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

	//Testing
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool m_Test_Run = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int m_Runs = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int m_Total = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float m_Avg_Error = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float m_Max_Avg_Error = 1;
	void Test_Runs();
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	float m_Accuracy = 1;
	void SetAccuracy(float val) {
		m_Accuracy = val;
	}
	UFUNCTION(BlueprintCallable)
	float GetAccuracy() {
		return m_Accuracy;
	}
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		ABrain* m_Brain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
		FVector m_Brain_Local_Location = {40, 0, 20};

	TMap<FString, int> m_Debug;
public:

	FVector Calculate_Robot_Space_Target_Location();

	ABrain* GetBrain();
	void SetBrain(ABrain* brain);

	void PlaceBrain();

	FMatrix GetPositionMatrix(FVector position);

	UFUNCTION(BlueprintCallable)
	FRobot_Pose_Report PoseRobot(FRobot_Pose pose);
	
	FMatrix RotationMatrix(RotationAxis axis, float theta);

	FMatrix DH_TranslationMatrix(DH_param dh);
};
