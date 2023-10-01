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
	m_Debug.Add("b_x_mat", 6);
	m_Debug.Add("x10", 7);
	m_Debug.Add("x11", 8);
	m_Debug.Add("x12", 9);
	m_Debug.Add("x13", 10);
	m_Debug.Add("target_l", 11);
	m_Debug.Add("x12", 9);
	m_Debug.Add("x13", 10);
	m_Debug.Add("target_f", 12);
	m_Debug.Add("b_pos", 13);
	m_Debug.Add("target_actual", 14);
	m_Debug.Add("target_l", 15);
	m_Debug.Add("entry_l", 16);
	m_Debug.Add("x_l", 17);
	m_Debug.Add("target_g", 18);
	m_Debug.Add("entry_g", 19);
	m_Debug.Add("x_g", 20);
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

	float b_t_deg = m_Brain->GetThetaDegrees();
	float b_t_rad = m_Brain->GetThetaRadians();

	FMatrix brain_rotation_matrix = m_Brain->GetRotationMatrix();
	GEngine->AddOnScreenDebugMessage(m_Debug["brain_rotation_matrix"], dur, color, FString::Printf(TEXT("brain_rotation_matrix : %s"), *brain_rotation_matrix.ToString()));
	auto brain_pos = m_Brain->GetActorLocation();

	//Local locations, therefore _l
	auto target_l = m_Brain->GetTargetLocalLocation();
	auto entry_l = m_Brain->GetEntryLocalLocation();
	auto x_l = m_Brain->GetEntryLocalLocation();

	//Identity matrices with the locations
	FMatrix target_mat = GetPositionMatrix(target_l);
	FMatrix entry_mat = GetPositionMatrix(entry_l);
	FMatrix x_mat = GetPositionMatrix(x_l);

	auto target_g_mat = brain_rotation_matrix * target_mat;
	auto target_g = target_g_mat.GetColumn(3) + brain_pos;

	auto entry_g_mat = brain_rotation_matrix * entry_mat;
	auto entry_g = entry_g_mat.GetColumn(3) + brain_pos;

	auto x_g_mat = brain_rotation_matrix * x_mat;
	auto x_g = x_g_mat.GetColumn(3) + brain_pos;

	GEngine->AddOnScreenDebugMessage(m_Debug["target_l"], dur, color, FString::Printf(TEXT("target_l : %s"), *target_l.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["entry_l"], dur, color, FString::Printf(TEXT("entry_l : %s"), *entry_l.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["x_l"], dur, color, FString::Printf(TEXT("x_l : %s"), *x_l.ToString()));


	GEngine->AddOnScreenDebugMessage(m_Debug["target_g"], dur, color, FString::Printf(TEXT("target_g : %s"), *target_g.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["entry_g"]	, dur, color, FString::Printf(TEXT("entry_g : %s"), *entry_g.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["x_g"]		, dur, color, FString::Printf(TEXT("x_g : %s"), *x_g.ToString()));

 	
	auto by = 0;
	auto bz = 0;
	//DELTA POSITION
	auto m_pos = GetActorLocation();

	auto d_pos = m_pos - brain_pos;
	GEngine->AddOnScreenDebugMessage(m_Debug["b_pos"], dur, color, FString::Printf(TEXT("brain_pos : %s"), *brain_pos.ToString()));
	
	GEngine->AddOnScreenDebugMessage(m_Debug["delta_position"], dur, color, FString::Printf(TEXT("delta_position : %s"), *d_pos.ToString()));

	auto target_actual = brain_pos + target_g;
	GEngine->AddOnScreenDebugMessage(m_Debug["target_actual"], dur, color, FString::Printf(TEXT("target_actual : %s"), *target_actual.ToString()));


}


ABrain* ARobot::GetBrain() {
	return m_Brain;
}
void ARobot::SetBrain(ABrain* brain) {
	m_Brain = brain;
}

FMatrix ARobot::GetPositionMatrix(FVector position)
{
	auto mat = FMatrix(
		FPlane(1, 0, 0, position.X),
		FPlane(0, 1, 0, position.Y),
		FPlane(0, 0, 1, position.Z),
		FPlane(0, 0, 0, 1)
	);

	return mat;
}
