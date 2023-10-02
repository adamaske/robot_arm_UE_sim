// Fill out your copyright notice in the Description page of Project Settings.


#include "Brain.h"
#include "Insertion_Target.h"
#include "Insertion_Target_Component.h"
// Sets default values
ABrain::ABrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Set Root

	m_Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SetRootComponent(m_Root);
	//2 Child actors

	m_Target = CreateDefaultSubobject<UInsertion_Target_Component>(TEXT("TARGET"));
	m_Target->SetupAttachment(RootComponent);

	m_Entry = CreateDefaultSubobject<UInsertion_Target_Component>(TEXT("ENTRY POINT"));
	m_Entry->SetupAttachment(RootComponent);

	m_X = CreateDefaultSubobject<UInsertion_Target_Component>(TEXT("X"));
	m_X->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void ABrain::BeginPlay()
{
	Super::BeginPlay();
	
	//Order targets
	m_Target->SetOrder(0);
	m_Entry->SetOrder(1);
	m_X->SetOrder(2);
	
	m_X_Local_Location = m_Entry_Local_Location + (m_Entry_Local_Location - m_Target_Local_Location) * m_X_Length;
	//Set positions
	m_Target->SetRelativeLocation(m_Target_Local_Location);
	m_Entry->SetRelativeLocation(m_Entry_Local_Location);
	m_X->SetRelativeLocation(m_X_Local_Location);
}

// Called every frame
void ABrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

