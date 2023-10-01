// Fill out your copyright notice in the Description page of Project Settings.


#include "Head.h"

// Sets default values
AHead::AHead()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHead::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ABrain* AHead::GetBrain() {
	return m_Brain;
}
void AHead::SetBrain(ABrain* brain) {
	m_Brain = brain;
}