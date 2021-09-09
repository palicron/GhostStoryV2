// Copyright Epic Games, Inc. All Rights Reserved.

#include "GhostStoryV2GameMode.h"
#include "GhostStoryV2HUD.h"
#include "GhostStoryV2Character.h"
#include "UObject/ConstructorHelpers.h"

AGhostStoryV2GameMode::AGhostStoryV2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGhostStoryV2HUD::StaticClass();
}
