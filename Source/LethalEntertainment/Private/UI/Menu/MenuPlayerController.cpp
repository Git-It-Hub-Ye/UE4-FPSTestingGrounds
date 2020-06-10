// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "MenuPlayerController.h"


void AMenuPlayerController::BeginPlay()
{
	if (!GetPawn()) { return; }
	UGameplayStatics::SpawnSoundAttached(SpawnSound, GetPawn()->GetRootComponent());
}

