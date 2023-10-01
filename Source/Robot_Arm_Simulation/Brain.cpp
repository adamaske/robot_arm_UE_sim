// Fill out your copyright notice in the Description page of Project Settings.


#include "Brain.h"

// Sets default values
ABrain::ABrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABrain::BeginPlay()
{
	Super::BeginPlay();

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
}

// Called every frame
void ABrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

