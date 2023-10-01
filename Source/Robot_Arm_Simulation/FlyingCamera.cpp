// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingCamera.h"

//Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

//Components
#include "Camera/CameraComponent.h"
//Movement
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
AFlyingCamera::AFlyingCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));


	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFlyingCamera::BeginPlay()
{
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(m_DefaultMappingContext, 0);

		}
	}
}

// Called every frame
void AFlyingCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFlyingCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//
		EnhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &AFlyingCamera::Move);
		EnhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Triggered, this, &AFlyingCamera::Look);

	}
}

void AFlyingCamera::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	auto Forward = GetActorForwardVector() * MovementVector.Y;
	auto Right = GetActorRightVector() * MovementVector.X;

	auto MoveSpeed = m_MoveSpeed * GetWorld()->GetDeltaSeconds();

	AddMovementInput(Forward, MoveSpeed);
	AddMovementInput(Right, MoveSpeed);

}

void AFlyingCamera::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	auto DeltaSeconds = GetWorld()->GetDeltaSeconds();

	float X = LookAxisVector.X * m_TurnRate * DeltaSeconds;
	float Y = LookAxisVector.Y * m_LookUpRate * DeltaSeconds;

	AddControllerYawInput(X);
	AddControllerPitchInput(Y);
}

