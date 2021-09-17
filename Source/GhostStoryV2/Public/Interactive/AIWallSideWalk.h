// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/ActionInteractive.h"
#include "AIWallSideWalk.generated.h"

class UBoxComponent;
UCLASS()
class GHOSTSTORYV2_API AAIWallSideWalk : public AActionInteractive
{
	GENERATED_BODY()

public:
	AAIWallSideWalk();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movment")
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player entry")
	class AGhostStoryV2Character* PlayerOverlap;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player entry")
	class UChildActorComponent* ActorPostLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player entry")
	class UChildActorComponent* ActorPostRight;


	bool bMovementToWall = false;
	bool bOnWallWalking = false;
	bool bOnWallExit = false;
	FVector EntryPos = FVector::ZeroVector;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void MoveToWall(float deltaSeconds);
	UFUNCTION()
		void ExitWall();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Action() override;

	virtual void DestroidAction() override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
