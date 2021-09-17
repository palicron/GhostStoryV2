// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionInteractive.generated.h"

UCLASS()
class GHOSTSTORYV2_API AActionInteractive : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActionInteractive();
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Movment")
	class UArrowComponent* DirectionalArrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movment")
		float NewCapsuleRadius = 30.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category="Movment")
	virtual void Action();
	UFUNCTION(BlueprintCallable, Category = "Movment")
	virtual void DestroidAction();

};
