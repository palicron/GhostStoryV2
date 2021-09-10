// Copyright Epic Games, Inc. All Rights Reserved.

#include "GhostStoryV2Character.h"
#include "GhostStoryV2Projectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AGhostStoryV2Character

AGhostStoryV2Character::AGhostStoryV2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	GetCharacterMovement()->MaxWalkSpeed = walkingBaseSpeed;

}

void AGhostStoryV2Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////
// Input

void AGhostStoryV2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);


	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AGhostStoryV2Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGhostStoryV2Character::MoveRight);


	PlayerInputComponent->BindAction("Spring", IE_Pressed, this, &AGhostStoryV2Character::StardSpring);
	PlayerInputComponent->BindAction("Spring", IE_Released, this, &AGhostStoryV2Character::EndSpring);
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGhostStoryV2Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGhostStoryV2Character::LookUpAtRate);
}

void AGhostStoryV2Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	
	
}


void AGhostStoryV2Character::MoveForward(float Value)
{

	
	if( FMath::Abs(GetVelocity().Size()) <=5.0f && !Running)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayWorldCameraShake(GetWorld(), Idle_Camara, GetActorLocation(), 100.0, 0.0, 0.0);
	}
	else if (FMath::Abs(GetVelocity().Size()) >= 5.0f && Running)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayWorldCameraShake(GetWorld(), Runing_Camera, GetActorLocation(), 100.0, 0.0, 0.0);
	}
	else
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayWorldCameraShake(GetWorld(), Walking_Camera, GetActorLocation(), 100.0, 0.0, 0.0);
	}
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AGhostStoryV2Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AGhostStoryV2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGhostStoryV2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGhostStoryV2Character::StardSpring()
{
	Running = true;
	GetCharacterMovement()->MaxWalkSpeed = walkingBaseSpeed* RuningMultiply;
}

void AGhostStoryV2Character::EndSpring()
{
	Running = false;
	GetCharacterMovement()->MaxWalkSpeed = walkingBaseSpeed / RuningMultiply;
}


