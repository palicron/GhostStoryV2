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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseLookUpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TSubclassOf<UCameraShakeBase> Idle_Camara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TSubclassOf<UCameraShakeBase> Walking_Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TSubclassOf<UCameraShakeBase> Runing_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float walkingBaseSpeed = 320.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float RuningMultiply = 1.9f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
		bool Running = false;

protected:
	
	float DeltaCamera = 0;
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

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


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
		void StardSpring();
	UFUNCTION(BlueprintCallable)
		void EndSpring();

public:

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

