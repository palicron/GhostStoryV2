// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GhostStoryV2Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UCameraShakeBase;
UENUM(BlueprintType)
enum class EMovementStay : uint8
{
	EM_Walk       UMETA(DisplayName = "Walking"),
	EM_Spring       UMETA(DisplayName = "Springting"),
	EM_Crouch       UMETA(DisplayName = "Crouching"),
	EM_Probe       UMETA(DisplayName = "Probing"),
	EM_Straff       UMETA(DisplayName = "Straffing"),
	EM_Climing       UMETA(DisplayName = "Climing"),
	EM_Lader       UMETA(DisplayName = "Lader")
};

UCLASS(config=Game)
class AGhostStoryV2Character : public ACharacter
{
	GENERATED_BODY()


		/** First person camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;


public:
	AGhostStoryV2Character();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float BaseLookUpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShakeBase> Idle_Camara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShakeBase> Walking_Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShakeBase> Runing_Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShakeBase> Crouch_Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShakeBase> Probe_Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShakeBase> WalkWalking_Camera;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		bool bCanControlCharacterForward = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		bool bCanControlCharacterRight = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		bool bCanTurnRate = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		bool bCanTurnUp = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		EMovementStay MovingplayerState = EMovementStay::EM_Walk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float walkingBaseSpeed = 320.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float RuningBaseSpeed = 450.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float CrouchingBaseSpeed = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float ProbeBaseSpeed = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float WallWalkBaseSpeed = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
		bool Running = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
		bool bProbeTimer = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float PressTimeToProbe = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float ProbeCapsuleRadius = 20.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float ProbeCapsuleMidheigt = 20.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float CapsuleRadius = 40.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float CapsuleMidheigt = 50.0f;

protected:

	FVector BaseCameraLocation;

	FVector CurrentInterpolatedLocation;
	
	float DeltaCamera = 0;

	float probeTimePress = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	FVector WallDirection = FVector::ZeroVector;
	
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);
	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void Tick(float DeltaSeconds) override;

	//Movement Functions

	void MoveForward(float Val);

	void MoveRight(float Val);
	
	UFUNCTION(BlueprintCallable)
		void StardSpring();
	UFUNCTION(BlueprintCallable)
		void EndSpring();

	UFUNCTION(BlueprintCallable)
	void StartCrouch();
	UFUNCTION(BlueprintCallable)
	void EndCrouch();

	UFUNCTION()
		virtual void OnStartCrouch(float HalfHeightAdjust,float ScaledHalfHeightAdjust) override;
	UFUNCTION()
		virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	UFUNCTION(BlueprintCallable)
		void Probe();

	virtual void AddControllerYawInput(float Val) override;

	UFUNCTION(BlueprintCallable)
		void SetCameraShake();


public:

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable)
	void SetMovementType(FVector BlockMovementVector = FVector(0,0,0),float lookUpAngelimit = 0.0f);


};

