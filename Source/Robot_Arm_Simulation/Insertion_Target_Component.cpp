// Fill out your copyright notice in the Description page of Project Settings.


#include "Insertion_Target_Component.h"

// Sets default values for this component's properties
UInsertion_Target_Component::UInsertion_Target_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInsertion_Target_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInsertion_Target_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

