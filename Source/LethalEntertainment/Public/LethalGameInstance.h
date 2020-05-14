// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "LethalGameInstance.generated.h"

class UUserWidget;
class UMenuWidget;

UCLASS()
class LETHALENTERTAINMENT_API ULethalGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
private:

	////////////////////////////////////////////////////////////////////////////////
	// Widget Variables

	/** BP widget for main menu */
	TSubclassOf<UUserWidget> MainMenuWidget;

	/** BP widget for in game menu */
	TSubclassOf<UUserWidget> InGameMenuWidget;

	/** BP widget for game over menu */
	TSubclassOf<UUserWidget> GameOverWidget;

	UPROPERTY()
	UMenuWidget * InGameMenu;

public:
	ULethalGameInstance(const FObjectInitializer & ObjectInitializer);

	////////////////////////////////////////////////////////////////////////////////
	// Display widget request functions

	UFUNCTION(BlueprintCallable)
	void DisplayMainMenu();

	void ToggleInGameMenu();

	void DisplayGameOverMenu();

protected:
	////////////////////////////////////////////////////////////////////////////////
	// Game action functions (Accessed through Menu interface)

	virtual void PlayGame() override;

	virtual void ReturnToMainMenu() override;

	virtual void OuitGame() override;

	virtual void ResumeGame() override;

	virtual void RestartGame() override;


private:
	////////////////////////////////////////////////////////////////////////////////
	// Load widget functions
	
	void LoadInGameMenu();

	void LoadGameOverMenu();

	void LoadMainMenu();

	void PauseGame();
	
};
