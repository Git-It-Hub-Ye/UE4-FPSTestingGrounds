// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "MenuPlayerController.h"
#include "MenuGameMode.h"


AMenuGameMode::AMenuGameMode()
{
	PrimaryActorTick.bCanEverTick = false;

	PlayerControllerClass = AMenuPlayerController::StaticClass();
}

void AMenuGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (NewPlayer == NULL) { return; }
	NewPlayer->GetPawn()->DisableInput(NewPlayer);
}

