// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "LethalGameInstance.generated.h"

class UUserWidget;
class UMenuWidget;

/** Basic data for weapon */
USTRUCT(BlueprintType)
struct FDefaultUserSettings {
	GENERATED_USTRUCT_BODY()

	/** Player default mouse sensitivity */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1.f, ClampMax = 100.f))
	float Default_MouseSens;

	/** Player default controller sensitivity */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1.f, ClampMax = 100.f))
	float Default_ConSens;

	/** Defaults */
	FDefaultUserSettings()
	{
		Default_MouseSens = 60;
		Default_ConSens = 60;
	}
};

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

	/** InGameMenu variable */
	UPROPERTY()
	UMenuWidget * InGameMenu;


	////////////////////////////////////////////////////////////////////////////////
	// Player Variables

	/** Struct contains default user settings (eg. Sensitivity) */
	FDefaultUserSettings DefaultUserSettings;

	/** Current sensitivity for mouse */
	float Sensitivity_Mouse;

	/** Current sensitivity for controller */
	float Sensitivity_Controller;

public:
	ULethalGameInstance(const FObjectInitializer & ObjectInitializer);


	////////////////////////////////////////////////////////////////////////////////
	// Display widget request functions

	UFUNCTION(BlueprintCallable)
	void DisplayMainMenu();

	void ToggleInGameMenu();

	void DisplayGameOverMenu();


	////////////////////////////////////////////////////////////////////////////////
	// Game data functions (can be accessed through Menu interface or other class)

	virtual float GetMouseSensitivity() override { return Sensitivity_Mouse; }

	virtual float GetControllerSensitivity() { return Sensitivity_Controller; }

protected:
	virtual void Init() override;

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
