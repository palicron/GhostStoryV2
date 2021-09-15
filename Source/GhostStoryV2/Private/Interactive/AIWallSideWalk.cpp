// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactive/AIWallSideWalk.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
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
	ActorPostRight->SetRelativeLocation(FVector::ZeroVector);
	ActorPostRight->SetRelativeRotation(FRotator::ZeroRotator);
}

void AAIWallSideWalk::BeginPlay()
{
	Super::BeginPlay();

}

void AAIWallSideWalk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR

	DrawDebugSphere(GetWorld(), GetActorLocation() + ActorPostLeft ->GetRelativeLocation() ,30, 10,FColor::Yellow, false, 0.1, 0, 2);
	DrawDebugSphere(GetWorld(), GetActorLocation() + ActorPostRight->GetRelativeLocation(), 30, 10, FColor::Cyan, false, 0.1, 0, 2);
#endif
	
}

void AAIWallSideWalk::Action()
{
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
			UE_LOG(LogTemp, Warning, TEXT("Derecha"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Izquierda"));
		}
	}
}

void AAIWallSideWalk::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerOverlap = Cast<AGhostStoryV2Character>(OtherActor);
	if (PlayerOverlap)
	{

	}
	else
	{
		PlayerOverlap = nullptr;
	}
}
