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
	m_Debug.Add("target_r", 21);
	m_Debug.Add("entry_r", 22);
	m_Debug.Add("x_r", 23);
	m_Debug.Add("ik_target_location", 24);
	m_Debug.Add("fk_end_effector_location", 25);
	m_Debug.Add("actual_end_effector_location", 26);
	m_Debug.Add("end_effector_offset", 27);
	m_Debug.Add("end_effector_error", 28);
	m_Debug.Add("fk_end_X", 29);
	m_Debug.Add("fk_end_Y", 30);
	m_Debug.Add("fk_end_Z", 31);
	PlaceBrain();
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
	auto r_pos = GetActorLocation();
	auto brain_pos = m_Brain->GetActorLocation();
	auto d_pos = r_pos - brain_pos;
	GEngine->AddOnScreenDebugMessage(m_Debug["b_pos"], dur, color, FString::Printf(TEXT("brain_pos : %s"), *brain_pos.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["delta_position"], dur, color, FString::Printf(TEXT("delta_position : %s"), *d_pos.ToString()));

	FMatrix brain_rotation_matrix = m_Brain->GetRotationMatrix();
	GEngine->AddOnScreenDebugMessage(m_Debug["brain_rotation_matrix"], dur, color, FString::Printf(TEXT("brain_rotation_matrix : %s"), *brain_rotation_matrix.ToString()));

	//Local locations, therefore _l
	auto target_l = m_Brain->GetTargetLocalLocation();
	auto entry_l = m_Brain->GetEntryLocalLocation();
	auto x_l = m_Brain->GetEntryLocalLocation();
	GEngine->AddOnScreenDebugMessage(m_Debug["target_l"], dur, color, FString::Printf(TEXT("target_l : %s"), *target_l.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["entry_l"], dur, color, FString::Printf(TEXT("entry_l : %s"), *entry_l.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["x_l"], dur, color, FString::Printf(TEXT("x_l : %s"), *x_l.ToString()));

	//Identity matrices with the locations
	FMatrix target_mat = GetPositionMatrix(target_l);
	FMatrix entry_mat = GetPositionMatrix(entry_l);
	FMatrix x_mat = GetPositionMatrix(x_l);

	//Global space 
	auto target_g_mat = brain_rotation_matrix * target_mat;
	auto target_g = target_g_mat.GetColumn(3) + brain_pos;

	auto entry_g_mat = brain_rotation_matrix * entry_mat;
	auto entry_g = entry_g_mat.GetColumn(3) + brain_pos;

	auto x_g_mat = brain_rotation_matrix * x_mat;
	auto x_g = x_g_mat.GetColumn(3) + brain_pos;
	GEngine->AddOnScreenDebugMessage(m_Debug["target_g"], dur, color, FString::Printf(TEXT("target_g : %s"), *target_g.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["entry_g"]	, dur, color, FString::Printf(TEXT("entry_g : %s"), *entry_g.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["x_g"]		, dur, color, FString::Printf(TEXT("x_g : %s"), *x_g.ToString()));

	//Robot space
	auto target_r = target_g - r_pos;
	auto entry_r = entry_g - r_pos;
	auto x_r = x_g - r_pos;
	GEngine->AddOnScreenDebugMessage(m_Debug["target_r"], dur, color, FString::Printf(TEXT("target_r : %s"), *target_r.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["entry_r"], dur, color, FString::Printf(TEXT("entry_r : %s"), *entry_r.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["x_r"], dur, color, FString::Printf(TEXT("x_r : %s"), *x_r.ToString()));

	auto ik_target_location = x_r;
	GEngine->AddOnScreenDebugMessage(m_Debug["ik_target_location"], dur, color, FString::Printf(TEXT("ik_target_location : %s"), *ik_target_location.ToString()));
}


ABrain* ARobot::GetBrain() {
	return m_Brain;
}
void ARobot::SetBrain(ABrain* brain) {
	m_Brain = brain;
}

void ARobot::PlaceBrain()
{
	if (!m_Brain) {
		return;
	}
	m_Brain->SetActorLocation(GetActorLocation() + m_Brain_Local_Location);


}


void ARobot::PoseRobot(FMatrix translation)
{
	// Solve ik function

	float d0 = 7.15;
	float d1 = 12.5;
	float d2 = 12.5;
	float d3 = 6.0;
	float d4 = 13.2;
	
	auto T1 = (RotationMatrix(Z, t0 - 180) * GetPositionMatrix(FVector(0, 0, d0)));
	auto T2 = (RotationMatrix(Y, t1) * GetPositionMatrix(FVector(0, 0, d1)));		
	auto T3 = (RotationMatrix(Y, t2) * GetPositionMatrix(FVector(0, 0, d2)));		
	auto T4 = (RotationMatrix(Y, t3) * GetPositionMatrix(FVector(0, 0, d3)));		
	auto T5 = (RotationMatrix(Z, t4) * GetPositionMatrix(FVector(0, 0, d4)));	

	FMatrix T = T1 * T2 * T3 * T4 * T5;
	FVector fk_end_effector_location = T.GetColumn(3);
	FVector fk_end_X = T.GetColumn(0);
	FVector fk_end_Y = T.GetColumn(1);
	FVector fk_end_Z = T.GetColumn(2);
	GEngine->AddOnScreenDebugMessage(m_Debug["fk_end_effector_location"], 15, FColor::Cyan, FString::Printf(TEXT("fk_end_effector_location : %s"), *fk_end_effector_location.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["fk_end_X"], 15, FColor::Cyan, FString::Printf(TEXT("fk_end_X : %s"), *fk_end_X.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["fk_end_Y"], 15, FColor::Cyan, FString::Printf(TEXT("fk_end_Y : %s"), *fk_end_Y.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["fk_end_Z"], 15, FColor::Cyan, FString::Printf(TEXT("fk_end_Z : %s"), *fk_end_Z.ToString()));

	// Apply
	m_Base->SetRelativeRotation(FRotator(0, t0, 0));
	m_Link_2->SetRelativeRotation(FRotator(t1, 0, 0));
	m_Link_3->SetRelativeRotation(FRotator(t2, 0, 0));
	m_Link_4->SetRelativeRotation(FRotator(t3, 0, 0));
	m_End_Effector->SetRelativeRotation(FRotator(0, t4, 0));

	//Find erros
	FVector actual_end_effector_location = m_End_Effector_Tip->GetComponentLocation() - GetActorLocation();// m_End_Effector->GetComponentLocation() - GetActorLocation();
	FVector end_effector_offset = actual_end_effector_location-fk_end_effector_location;
	float end_effector_error = end_effector_offset.Length(); 

	GEngine->AddOnScreenDebugMessage(m_Debug["actual_end_effector_location"], 15, FColor::Cyan, FString::Printf(TEXT("actual_end_effector_location : %s"), *actual_end_effector_location.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["end_effector_offset"], 15, FColor::Cyan, FString::Printf(TEXT("end_effector_offset : %s"), *end_effector_offset.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["end_effector_error"], 15, FColor::Cyan, FString::Printf(TEXT("end_effector_error : %f"), end_effector_error));



}

FMatrix ARobot::RotationMatrix(RotationAxis axis, float theta)
{
	FMatrix m;
	auto ct = FMath::Cos(FMath::DegreesToRadians(theta));
	auto st = FMath::Sin(FMath::DegreesToRadians(theta));

	switch (axis) {
	case(RotationAxis::X):
		m = FMatrix(
			FPlane(1, 0, 0, 0),
			FPlane(0, ct, -st, 0),
			FPlane(0, st, ct, 0),
			FPlane(0, 0, 0, 1)
		);
		break;
	case(RotationAxis::Y):
		m = FMatrix(
			FPlane(ct, 0, st, 0),
			FPlane(0, 1, 0, 0),
			FPlane(-st, 0, ct, 0),
			FPlane(0, 0, 0, 1)
		);
		break; 
	case(RotationAxis::Z):
		m = FMatrix(
			FPlane(ct, -st, 0, 0),
			FPlane(st, ct, 0, 0),
			FPlane(0, 0, 1, 0),
			FPlane(0, 0, 0, 1)
		);
			break;
	case(RotationAxis::I):
		m = FMatrix(
			FPlane(1, 0, 0, 0),
			FPlane(0, 1, 0, 0),
			FPlane(0, 0, 1, 0),
			FPlane(0, 0, 0, 1)
		);
		break;
	}
	

	return m;
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
