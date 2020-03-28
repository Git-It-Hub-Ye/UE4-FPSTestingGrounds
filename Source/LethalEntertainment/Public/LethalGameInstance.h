// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "LethalGameInstance.generated.h"

UCLASS()
class LETHALENTERTAINMENT_API ULethalGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
private:
	/** BP widget for main menu */
	TSubclassOf<class UUserWidget> MainMenuWidget;

	/** BP widget for in game menu */
	TSubclassOf<class UUserWidget> InGameMenuWidget;

	class UMenuWidget * InGameMenu;

public:
	ULethalGameInstance(const FObjectInitializer & ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	void CleanupSessionOnReturnToMenu();

protected:
	virtual void PlayGame() override;

	virtual void OuitGame() override;
	
	
};
