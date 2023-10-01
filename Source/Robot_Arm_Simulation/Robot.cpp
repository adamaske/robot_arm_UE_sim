// Fill out your copyright notice in the Description page of Project Settings.


#include "Robot.h"
#include "Brain.h"

// Sets default values
ARobot::ARobot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARobot::BeginPlay()
{
	Super::BeginPlay();

	m_Debug.Add("No Brain", 1);
	m_Debug.Add("brain_y_rot", 2);
	m_Debug.Add("delta_position", 3);
	m_Debug.Add("brain_rotation_matrix", 4);
	m_Debug.Add("t_matrix", 5);
}

// Called every frame
void ARobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_Brain) {
		Calculate();
	}
}

void ARobot::Calculate() {

	if (!m_Brain) {
		// NO BRAIN
		GEngine->AddOnScreenDebugMessage(m_Debug["No Brain"], 5, FColor::Red, FString::Printf(TEXT("NO BRAIN FOUND")));
		return;
	}	
	FColor color = FColor::Cyan;
	float dur = 5;

	float unit_scale = 1; //brain 2 robot unit scale
	//DELTA POSITION
	auto mpos = GetActorLocation();
	auto bpos = m_Brain->GetActorLocation();
	auto dpos = mpos - bpos;
	GEngine->AddOnScreenDebugMessage(m_Debug["delta_position"], dur, color, FString::Printf(TEXT("delta_position : %s"), *dpos.ToString()));


	float brain_theta = m_Brain->GetThetaDegrees();
	float b_t_rad = FMath::DegreesToRadians(brain_theta);

	FMatrix brain_rotation_matrix = m_Brain->GetRotationMatrix();
	GEngine->AddOnScreenDebugMessage(m_Debug["brain_rotation_matrix"], dur, color, FString::Printf(TEXT("brain_rotation_matrix : %s"), *brain_rotation_matrix.ToString()));

	auto m_rot_mat = FMatrix(
		FPlane(1,0,0,0),
		FPlane(0,1,0,0),
		FPlane(0,0,1,0),
		FPlane(0,0,0,1)
	);

	auto t = brain_rotation_matrix * m_rot_mat;
	GEngine->AddOnScreenDebugMessage(m_Debug["t_matrix"], dur, color, FString::Printf(TEXT("t_matrix : %s"), *t.ToString()));

}


ABrain* ARobot::GetBrain() {
	return m_Brain;
}
void ARobot::SetBrain(ABrain* brain) {
	m_Brain = brain;
}