// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "LethalEntertainmentGameMode.h"
#include "LethalEntertainmentHUD.h"

ALethalEntertainmentGameMode::ALethalEntertainmentGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Dynamic/Character/Behaviour/BP_PlayerPawn"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ALethalEntertainmentHUD::StaticClass();
}

