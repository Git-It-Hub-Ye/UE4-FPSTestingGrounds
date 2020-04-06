// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

class USoundBase;

/**
 * Player controller for main menu
 */
UCLASS()
class LETHALENTERTAINMENT_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, DisplayName = "Spawn Sound on Start", Category = "Audio")
	USoundBase * SpawnSound;

protected:

	virtual void BeginPlay() override;
		
};
