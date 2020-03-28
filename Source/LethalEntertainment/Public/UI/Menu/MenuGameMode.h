// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

UCLASS()
class LETHALENTERTAINMENT_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMenuGameMode();

	virtual void PostLogin(APlayerController * NewPlayer) override;
};
