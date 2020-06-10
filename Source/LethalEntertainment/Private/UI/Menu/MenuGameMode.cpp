// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "MenuPlayerController.h"
#include "MenuGameMode.h"


AMenuGameMode::AMenuGameMode()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerConClassFinder(TEXT("/Game/Dynamic/Blueprints/BP_Menu_PC"));
	PlayerControllerClass = PlayerConClassFinder.Class;
}

void AMenuGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (NewPlayer == NULL) { return; }
	NewPlayer->GetPawn()->DisableInput(NewPlayer);
}

