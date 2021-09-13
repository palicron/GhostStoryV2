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


	BaseCameraLocation = GetFirstPersonCameraComponent()->GetRelativeLocation();
	
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

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AGhostStoryV2Character::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AGhostStoryV2Character::EndCrouch);
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AGhostStoryV2Character::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGhostStoryV2Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGhostStoryV2Character::LookUpAtRate);
}

void AGhostStoryV2Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	CurrentInterpolatedLocation = FMath::VInterpTo(CurrentInterpolatedLocation, BaseCameraLocation, DeltaSeconds, 10.0f);
	GetFirstPersonCameraComponent()->SetRelativeLocation(CurrentInterpolatedLocation);	
	if(bProbeTimer && MovingplayerState != EMovementStay::EM_Probe)
	{
		probeTimePress += DeltaSeconds;
		if(probeTimePress>=PressTimeToProbe)
		{
			Probe();
		}
	}
	SetCameraShake();
	
}


void AGhostStoryV2Character::MoveForward(float Value)
{
	

	if (!bCanControlCharacterForward)
		return;
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AGhostStoryV2Character::MoveRight(float Value)
{

	if (!bCanControlCharacterRight)
		return;
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AGhostStoryV2Character::TurnAtRate(float Rate)
{
	if (!bCanTurnRate)
		return;
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGhostStoryV2Character::LookUpAtRate(float Rate)
{
	if (!bCanTurnUp)
		return
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGhostStoryV2Character::StardSpring()
{
	if(MovingplayerState== EMovementStay::EM_Spring || MovingplayerState == EMovementStay::EM_Walk)
	{
		MovingplayerState = EMovementStay::EM_Spring;
		Running = true;
		SetMovementType();
	}

}

void AGhostStoryV2Character::EndSpring()
{
	if(MovingplayerState == EMovementStay::EM_Spring)
	{
		MovingplayerState = EMovementStay::EM_Walk;
		Running = false;
		SetMovementType();
	}

}

void AGhostStoryV2Character::StartCrouch()
{
	if(!GetCharacterMovement()->IsCrouching())
	{
		GetCharacterMovement()->MaxWalkSpeedCrouched = 200.f;
		GetCharacterMovement()->bWantsToCrouch = true;
		GetCharacterMovement()->Crouch();
		if(GetCharacterMovement()->IsCrouching())
		{
			MovingplayerState = EMovementStay::EM_Crouch;
			bProbeTimer = true;
			probeTimePress = 0;
			SetMovementType();
		}
	
	}
	else
	{
		
	
		GetCharacterMovement()->bWantsToCrouch = false;
		GetCharacterMovement()->UnCrouch();
		if(!bIsCrouched)
		{
			MovingplayerState = EMovementStay::EM_Walk;
			GetCapsuleComponent()->SetCapsuleRadius(CapsuleRadius);
			GetCharacterMovement()->CrouchedHalfHeight = CapsuleMidheigt;
			SetMovementType();
	
		}
		bProbeTimer = false;
	
	}
}

void AGhostStoryV2Character::EndCrouch()
{
	bProbeTimer = false;
	
	
}

void AGhostStoryV2Character::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{

	if(!GetCharacterMovement()->IsFalling())
	{
		
		CurrentInterpolatedLocation = BaseCameraLocation + FVector(0, 0, ScaledHalfHeightAdjust);
	}
	
}

void AGhostStoryV2Character::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (!GetCharacterMovement()->IsFalling())
	{
	CurrentInterpolatedLocation = BaseCameraLocation - FVector(0, 0, ScaledHalfHeightAdjust);
	}
}

void AGhostStoryV2Character::Probe()
{

	if (MovingplayerState==EMovementStay::EM_Crouch)
	{
	 
		
		GetCapsuleComponent()->SetCapsuleRadius(ProbeCapsuleRadius);
		GetCharacterMovement()->CrouchedHalfHeight = ProbeCapsuleMidheigt;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 150.f;
		GetCharacterMovement()->bWantsToCrouch = true;
		GetCharacterMovement()->Crouch();
		MovingplayerState = EMovementStay::EM_Probe;
		SetMovementType();
	}
}

void AGhostStoryV2Character::AddControllerYawInput(float Val)
{
	//if (!bCanTurnRate)
	//	return;
	Super::AddControllerYawInput(Val);
}



void AGhostStoryV2Character::SetMovementType( FVector BlockMovementVector, float lookUpAngelimit)
{
	switch (MovingplayerState)
	{
	case EMovementStay::EM_Walk:
		bCanControlCharacterForward = true;
		bCanControlCharacterRight = true;
		bCanTurnRate = true;
		bCanTurnUp = true;
		GetCharacterMovement()->MaxWalkSpeed = walkingBaseSpeed;
		break;
	case EMovementStay::EM_Spring:
		bCanControlCharacterForward = true;
		bCanControlCharacterRight = true;
		bCanTurnRate = true;
		bCanTurnUp = true;
		GetCharacterMovement()->MaxWalkSpeed = RuningBaseSpeed;
		break;
	case EMovementStay::EM_Crouch:
		bCanControlCharacterForward = true;
		bCanControlCharacterRight = true;
		bCanTurnRate = true;
		bCanTurnUp = true;
		GetCharacterMovement()->MaxWalkSpeed = CrouchingBaseSpeed;
		break;
	case EMovementStay::EM_Probe:
		bCanControlCharacterForward = true;
		bCanControlCharacterRight = true;
		bCanTurnRate = true;
		bCanTurnUp = true;
		GetCharacterMovement()->MaxWalkSpeed = ProbeBaseSpeed;
		break;
	case EMovementStay::EM_Straff:
		bCanControlCharacterForward = false;
		bCanControlCharacterRight = true;
		bCanTurnRate = false;
		bCanTurnUp = false;
		GetCharacterMovement()->MaxWalkSpeed = WallWalkBaseSpeed;
		if(BlockMovementVector!=FVector::ZeroVector)
		{
			WallDirection = BlockMovementVector.GetSafeNormal();
		}
		break;
	case EMovementStay::EM_Climing:
		break;
	case EMovementStay::EM_Lader:
		break;
	}
}

void AGhostStoryV2Character::SetCameraShake()
{
	if (FMath::Abs(GetVelocity().Size()) <= 5.0f)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayWorldCameraShake(GetWorld(), Idle_Camara, GetActorLocation(), 100.0, 0.0, 0.0);
	}
	else
	{
		switch (MovingplayerState)
		{
		case EMovementStay::EM_Walk:
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayWorldCameraShake(GetWorld(), Walking_Camera, GetActorLocation(), 100.0, 0.0, 0.0);
			break;
		case EMovementStay::EM_Spring:
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayWorldCameraShake(GetWorld(), Runing_Camera, GetActorLocation(), 100.0, 0.0, 0.0);
			break;
		case EMovementStay::EM_Crouch:
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayWorldCameraShake(GetWorld(), Crouch_Camera, GetActorLocation(), 100.0, 0.0, 0.0);
			break;
		case EMovementStay::EM_Probe:
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayWorldCameraShake(GetWorld(), Probe_Camera, GetActorLocation(), 100.0, 0.0, 0.0);
			break;
		case EMovementStay::EM_Straff:
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayWorldCameraShake(GetWorld(), WalkWalking_Camera, GetActorLocation(), 100.0, 0.0, 0.0);
			break;
		case EMovementStay::EM_Climing:
			break;
		case EMovementStay::EM_Lader:
			break;
		}
	}
}





