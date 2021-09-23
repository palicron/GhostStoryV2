// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactive/AILadder.h"
#include "Components/BoxComponent.h"
#include "GhostStoryV2/GhostStoryV2Character.h"

AAILadder::AAILadder()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ladder Mesh"));
	RootComponent = Mesh;
	BottomBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Bottom Collider"));
	UpperBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Upper Collider"));
	BottomBox->SetupAttachment(RootComponent);
	UpperBox->SetupAttachment(RootComponent);

	BottomBox->OnComponentBeginOverlap.AddDynamic(this,&AAILadder::OnOverlapBeginBottom);
	BottomBox->OnComponentEndOverlap.AddDynamic(this,&AAILadder::OnOverlapEndBottom );

	UpperBox->OnComponentBeginOverlap.AddDynamic(this, &AAILadder::OnOverlapBeginUpper);
	UpperBox->OnComponentEndOverlap.AddDynamic(this, &AAILadder::OnOverlapEndUpper);
	
}


void AAILadder::BeginPlay()
{
	Super::BeginPlay();
}

void AAILadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAILadder::Action()
{
	
	if (PlayerOverlap && !OnLadder)
	{
		PlayerOverlap->MovingplayerState = EMovementStay::EM_Lader;
		PlayerOverlap->SetMovementType();
		FVector posnew = PlayerOverlap->GetActorLocation();
		posnew.Z += 50.0f;
		PlayerOverlap->SetActorLocation(posnew);
		OnLadder = true;	
	}
}

void AAILadder::DestroidAction()
{
	PlayerOverlap->MovingplayerState = EMovementStay::EM_Walk;
	PlayerOverlap->SetMovementType();
	OnLadder = false;
}


void AAILadder::OnOverlapBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerOverlap = Cast<AGhostStoryV2Character>(OtherActor);
	if (PlayerOverlap && !OnLadder)
	{
		intro = 0;
		PlayerOverlap->CurrentInteractive = this;
		PlayerOverlap->WallDirection = GetActorLocation();
	}
}

void AAILadder::OnOverlapEndBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGhostStoryV2Character* exit = Cast<AGhostStoryV2Character>(OtherActor);
	if (exit && PlayerOverlap && !OnLadder)
	{
		PlayerOverlap->CurrentInteractive = nullptr;
		PlayerOverlap->WallDirection = FVector::ZeroVector;
	}
	else if(exit && PlayerOverlap && OnLadder)
	{
		
	}
		
}

void AAILadder::OnOverlapBeginUpper(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerOverlap = Cast<AGhostStoryV2Character>(OtherActor);
	if (PlayerOverlap && !OnLadder)
	{
		intro = 1;
		PlayerOverlap->CurrentInteractive = this;
		PlayerOverlap->WallDirection = GetActorLocation();
	}
}

void AAILadder::OnOverlapEndUpper(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGhostStoryV2Character* exit = Cast<AGhostStoryV2Character>(OtherActor);
	if (exit && PlayerOverlap && !OnLadder)
	{
		PlayerOverlap->CurrentInteractive = nullptr;
		PlayerOverlap->WallDirection = FVector::ZeroVector;
	
	}
	else if (exit && PlayerOverlap && OnLadder)
	{
		
	}
}
