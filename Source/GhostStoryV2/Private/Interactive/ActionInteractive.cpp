// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactive/ActionInteractive.h"
#include "Components/ArrowComponent.h"

// Sets default values
AActionInteractive::AActionInteractive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DirectionalArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Directional Arrow"));
	RootComponent = DirectionalArrow;
}

// Called when the game starts or when spawned
void AActionInteractive::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActionInteractive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActionInteractive::Action()
{
	
}

void AActionInteractive::DestroidAction()
{
}

