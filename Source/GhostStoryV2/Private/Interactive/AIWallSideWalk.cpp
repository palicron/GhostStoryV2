// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactive/AIWallSideWalk.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GhostStoryV2/GhostStoryV2Character.h"
#include "Components/ChildActorComponent.h"
#if WITH_EDITOR
#include  "DrawDebugHelpers.h"
#endif

AAIWallSideWalk::AAIWallSideWalk()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collition"));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AAIWallSideWalk::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AAIWallSideWalk::OnOverlapEnd);

	ActorPostLeft = CreateDefaultSubobject<UChildActorComponent>(TEXT("Left Entry"));
	ActorPostRight = CreateDefaultSubobject<UChildActorComponent>(TEXT("Right Entry"));
	ActorPostLeft->SetRelativeLocation(FVector::ZeroVector);
	ActorPostLeft->SetRelativeRotation(FRotator::ZeroRotator);
	const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	ActorPostRight->SetWorldLocationAndRotation(GetActorLocation(), GetActorRotation());
	//ActorPostRight->SetRelativeRotation(FRotator::ZeroRotator);
	ActorPostLeft->AttachToComponent(BoxCollision, AttachmentTransformRules);
	ActorPostRight->AttachToComponent(BoxCollision, AttachmentTransformRules);
}

void AAIWallSideWalk::BeginPlay()
{
	Super::BeginPlay();

}



void AAIWallSideWalk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bMovementToWall)
	{
		MoveToWall(DeltaTime);
	}
	else if(bOnWallExit)
	{
		
	}
	

#if WITH_EDITOR
	
	DrawDebugSphere(GetWorld(),  ActorPostLeft ->K2_GetComponentLocation(),30, 10,FColor::Yellow, false, 0.1, 0, 2);
	DrawDebugSphere(GetWorld(), ActorPostRight->K2_GetComponentLocation(), 30, 10, FColor::Cyan, false, 0.1, 0, 2);
#endif
	
}

void AAIWallSideWalk::Action()
{
	if(PlayerOverlap && (!bMovementToWall && !bOnWallWalking))
	{
		PlayerOverlap->GetCapsuleComponent()->SetCapsuleRadius(NewCapsuleRadius);
		PlayerOverlap->MovingplayerState = EMovementStay::EM_Straff;
		PlayerOverlap->SetMovementType();
		FInputModeUIOnly input;
		GetWorld()->GetFirstPlayerController()->SetInputMode(input);
		bMovementToWall = true;
	}
}

void AAIWallSideWalk::DestroidAction()
{
}

void AAIWallSideWalk::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerOverlap = Cast<AGhostStoryV2Character>(OtherActor);
	if(PlayerOverlap)
	{
		if(FVector::CrossProduct(DirectionalArrow->GetForwardVector(), GetActorLocation()- PlayerOverlap->GetActorLocation()).Z<0)
		{
			EntryPos = ActorPostRight->K2_GetComponentLocation();
			
			UE_LOG(LogTemp, Warning, TEXT("Derecha"));
		}
		else
		{
			EntryPos = ActorPostLeft->K2_GetComponentLocation();
			UE_LOG(LogTemp, Warning, TEXT("Izquierda"));
		}
		PlayerOverlap->CurrentInteractive = this;
		PlayerOverlap->WallDirection = DirectionalArrow->GetRightVector().GetSafeNormal();
	}
}

void AAIWallSideWalk::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerOverlap = Cast<AGhostStoryV2Character>(OtherActor);
	if (PlayerOverlap)
	{
		EntryPos = FVector::ZeroVector;
		PlayerOverlap->CurrentInteractive = nullptr;
		PlayerOverlap = nullptr;
		bMovementToWall = false;
		bOnWallWalking = false;
	
		
	}
	
    	
	
}

void AAIWallSideWalk::MoveToWall(float deltaSeconds)
{
	if(PlayerOverlap)
	{
		
		if(FVector::DotProduct(PlayerOverlap->GetActorForwardVector(), DirectionalArrow->GetForwardVector()) <=0.98)
		{
			if (FVector::CrossProduct(PlayerOverlap->GetActorForwardVector(), DirectionalArrow->GetForwardVector()).Z < 0)
			{
				GetWorld()->GetFirstPlayerController()->AddYawInput(-1);
			}
			else
			{
				GetWorld()->GetFirstPlayerController()->AddYawInput(1);
			}
		}
		EntryPos.Z = PlayerOverlap->GetActorLocation().Z;	
		if(FVector::Dist(PlayerOverlap->GetActorLocation(), EntryPos)>=5.0f)
		{
			PlayerOverlap->SetActorLocation(FMath::VInterpTo(PlayerOverlap->GetActorLocation(), EntryPos, deltaSeconds, 8.0f));
		}
		else
		{
			FInputModeGameOnly input;
			GetWorld()->GetFirstPlayerController()->SetInputMode(input);
			bMovementToWall = false;
			bOnWallWalking = true;
			PlayerOverlap->bCanTurnRate = true;
			PlayerOverlap->bCanTurnUp = true;
		}
	}
}

void AAIWallSideWalk::ExitWall()
{
}
