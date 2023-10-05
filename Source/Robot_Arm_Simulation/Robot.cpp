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
	m_Debug.Add("fk_end_x", 29);
	m_Debug.Add("fk_end_y", 30);
	m_Debug.Add("fk_end_z", 31);
	m_Debug.Add("dh_pos", 32);
	m_Debug.Add("acutal_end_effector_x", 33);
	m_Debug.Add("acutal_end_effector_y", 34);
	m_Debug.Add("acutal_end_effector_z", 35);
	m_Debug.Add("ik_t0", 36);
	m_Debug.Add("dh_fk_error", 37); 
	
	m_Debug.Add("dh_0", 38); 
	m_Debug.Add("dh_1", 39); 
	m_Debug.Add("dh_2", 40); 
	m_Debug.Add("dh_3", 41);
	
	
	
	
	PlaceBrain();
}

// Called every frame
void ARobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (m_Brain) {
	//	Calculate();
	//}
}

FVector ARobot::Calculate_Robot_Space_Target_Location() {

	if (!m_Brain) {
		// NO BRAIN
		GEngine->AddOnScreenDebugMessage(m_Debug["No Brain"], 5, FColor::Red, FString::Printf(TEXT("NO BRAIN FOUND")));
		return FVector{ 0,0,0 };
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
	auto target_m = GetPositionMatrix(ik_target_location);
	return ik_target_location;
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
	//Find robot_space target location
	FVector target = Calculate_Robot_Space_Target_Location();

	//Adjustments
	auto _t0 = t0;// + 180;// +180;//0;
	auto _t1 = t1;//-90;//+ -90;
	auto _t2 = t2;//-90;// - 90;
	auto _t3 = t3;//-90;// - 90;
	auto _t4 = t4;//;// -90;// +0;
						
	float d0 = 7.15;
	float d1 = 12.5;
	float d2 = 12.5;
	float d3 = 6.0;
	float d4 = 13.2;
	
	//Translation matrices
	auto T1 = (RotationMatrix(Z, _t0) * GetPositionMatrix(FVector(0, 0, d0)));
	auto T2 = (RotationMatrix(Y, _t1) * GetPositionMatrix(FVector(0, 0, d1)));		
	auto T3 = (RotationMatrix(Y, _t2) * GetPositionMatrix(FVector(0, 0, d2)));		
	auto T4 = (RotationMatrix(Y, _t3) * GetPositionMatrix(FVector(0, 0, d3)));		
	auto T5 = (RotationMatrix(Z, _t4) * GetPositionMatrix(FVector(0, 0, d4)));	
	FMatrix T = T1 * T2 * T3 * T4 * T5;

	//Forward Kinematics
	FVector fk_end_effector_location = T.GetColumn(3);
	FVector fk_end_x = T.GetColumn(0);
	FVector fk_end_y = T.GetColumn(1);
	FVector fk_end_z = T.GetColumn(2);
	GEngine->AddOnScreenDebugMessage(m_Debug["fk_end_effector_location"], 15, FColor::Cyan, FString::Printf(TEXT("fk_end_effector_location : %s"), *fk_end_effector_location.ToString()));
	//GEngine->AddOnScreenDebugMessage(m_Debug["fk_end_x"], 15, FColor::Cyan, FString::Printf(TEXT("fk_end_X : %s"), *fk_end_x.ToString()));
	//GEngine->AddOnScreenDebugMessage(m_Debug["fk_end_y"], 15, FColor::Cyan, FString::Printf(TEXT("fk_end_Y : %s"), *fk_end_y.ToString()));
	//GEngine->AddOnScreenDebugMessage(m_Debug["fk_end_z"], 15, FColor::Cyan, FString::Printf(TEXT("fk_end_Z : %s"), *fk_end_z.ToString()));

	//Set the joint angles
	m_Base->SetRelativeRotation(FRotator(0, t0, 0));
	m_Link_2->SetRelativeRotation(FRotator(t1, 0, 0));
	m_Link_3->SetRelativeRotation(FRotator(t2, 0, 0));
	m_Link_4->SetRelativeRotation(FRotator(t3, 0, 0));
	m_End_Effector->SetRelativeRotation(FRotator(90, t4, 0));

	//Location
	FVector actual_end_effector_location = m_End_Effector_Tip->GetComponentLocation() - GetActorLocation();// m_End_Effector->GetComponentLocation() - GetActorLocation();
	FVector end_effector_offset = actual_end_effector_location-fk_end_effector_location;
	float end_effector_error = end_effector_offset.Length(); 
	GEngine->AddOnScreenDebugMessage(m_Debug["actual_end_effector_location"], 15, FColor::Cyan, FString::Printf(TEXT("actual_end_effector_location : %s"), *actual_end_effector_location.ToString()));
	//GEngine->AddOnScreenDebugMessage(m_Debug["end_effector_offset"], 15, FColor::Cyan, FString::Printf(TEXT("end_effector_offset : %s"), *end_effector_offset.ToString()));
	//GEngine->AddOnScreenDebugMessage(m_Debug["end_effector_error"], 15, FColor::Cyan, FString::Printf(TEXT("end_effector_error : %f"), end_effector_error));
	
	//Rotation
	FVector acutal_end_effector_x = m_End_Effector_Tip->GetRightVector();
	FVector acutal_end_effector_y = m_End_Effector_Tip->GetUpVector();
	FVector acutal_end_effector_z = m_End_Effector_Tip->GetForwardVector();
	//GEngine->AddOnScreenDebugMessage(m_Debug["acutal_end_effector_x"], 15, FColor::Cyan, FString::Printf(TEXT("acutal_end_effector_x : %s"), *acutal_end_effector_x.ToString()));
	//GEngine->AddOnScreenDebugMessage(m_Debug["acutal_end_effector_y"], 15, FColor::Cyan, FString::Printf(TEXT("acutal_end_effector_y : %s"), *acutal_end_effector_y.ToString()));
	//GEngine->AddOnScreenDebugMessage(m_Debug["acutal_end_effector_z"], 15, FColor::Cyan, FString::Printf(TEXT("acutal_end_effector_z : %s"), *acutal_end_effector_z.ToString()));

	//Inverse Kinematics

	//Denavit Hartenberg parameters
	DH_param dh_p_0 = {t0, -90, 0, 7.15 };
	DH_param dh_p_1 = {_t1,	0, 12.5, 0};
	DH_param dh_p_2 = {_t2, 0, 12.5, 0 };
	DH_param dh_p_3 = {_t3, 0, 19.2, 0};
	
	//DH translation matrices
	auto dh0 = DH_TranslationMatrix(dh_p_0);
	auto dh1 = DH_TranslationMatrix(dh_p_1);
	auto dh2 = DH_TranslationMatrix(dh_p_2);
	auto dh3 = DH_TranslationMatrix(dh_p_3);
	auto dh_T = dh0 * dh1 * dh2 *dh3;// *dh1* dh2* dh3;

	//End location DH FK model
	auto dh_pos = dh_T.GetColumn(3);
	GEngine->AddOnScreenDebugMessage(m_Debug["dh_pos"], 15, FColor::Cyan, FString::Printf(TEXT("dh_pos : %s"), *dh_pos.ToString()));

	auto dh_0 = (dh0).GetColumn(3);
	auto dh_1 = (dh0*dh1).GetColumn(3);
	auto dh_2 = (dh0*dh1*dh2).GetColumn(3);
	auto dh_3 = (dh0 * dh1 * dh2*dh3).GetColumn(3);
	GEngine->AddOnScreenDebugMessage(m_Debug["dh_0"], 15, FColor::Cyan, FString::Printf(TEXT("dh_0 : %s"), *dh_0.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["dh_1"], 15, FColor::Cyan, FString::Printf(TEXT("dh_1 : %s"), *dh_1.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["dh_2"], 15, FColor::Cyan, FString::Printf(TEXT("dh_2 : %s"), *dh_2.ToString()));
	GEngine->AddOnScreenDebugMessage(m_Debug["dh_3"], 15, FColor::Cyan, FString::Printf(TEXT("dh_3 : %s"), *dh_3.ToString()));

	//auto dh_pos = dh_T.GetColumn(3);
	
	//Find error
	auto dh_fk_error = (actual_end_effector_location - dh_pos).Length();
	GEngine->AddOnScreenDebugMessage(m_Debug["dh_fk_error"], 15, FColor::Cyan, FString::Printf(TEXT("dh_fk_error : %f"), dh_fk_error));

	float p_r = FMath::Sqrt((target.X * target.X) + (target.Y * target.Y));
	auto r_3 = p_r;
	auto z_3 = target.Z - d0;

	//phi = t1 + t2 + t3
	//auto phi = 0 + 0 + 0;
	//auto r_2 = r_3 - 
	//auto t_3 = FMath::Acos(()/ (2*dh_p_1.alpha*))
	////t0 = found
	//if X = 5 and Y = 3, then t0 = atan2(5, 3);
	float ik_t0 = FMath::Atan2(fk_end_effector_location.X, fk_end_effector_location.Y);
	GEngine->AddOnScreenDebugMessage(m_Debug["ik_t0"], 15, FColor::Cyan, FString::Printf(TEXT("ik_t0 : %f"), ik_t0));

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

FMatrix ARobot::DH_TranslationMatrix(DH_param dh)
{
	auto t = FMath::DegreesToRadians(dh.t);
	auto alpha =FMath::DegreesToRadians( dh.alpha);
	auto r = dh.r;
	auto d = dh.d;

	auto ct = cos(t);
	auto st = sin(t);

	auto mat = FMatrix(
		FPlane(ct, -st*cos(alpha), st*sin(alpha), r*ct),
		FPlane(st, ct*cos(alpha), -ct*sin(alpha), r*st),
		FPlane(0, st*alpha, ct*alpha, d),
		FPlane(0,0,0,1)
	
	);
	return mat;
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
