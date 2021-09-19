// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/ActionInteractive.h"
#include "AILadder.generated.h"
class UBoxComponent;
/**
 * 
 */
UCLASS()
class GHOSTSTORYV2_API AAILadder : public AActionInteractive
{
	GENERATED_BODY()

public:
	AAILadder();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player entry")
		class AGhostStoryV2Character* PlayerOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movment")
		UBoxComponent* BottomBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movment")
		UBoxComponent* UpperBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ladder")
		bool OnLadder = false;
protected:
	virtual void BeginPlay() override;
	//0 para bottom y 1 para upper;
	uint8 intro = 0;
public:

	virtual void Tick(float DeltaTime) override;

	virtual void Action() override;

	virtual void DestroidAction() override;
	
	UFUNCTION()
		void OnOverlapBeginBottom(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEndBottom(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnOverlapBeginUpper(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEndUpper(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
