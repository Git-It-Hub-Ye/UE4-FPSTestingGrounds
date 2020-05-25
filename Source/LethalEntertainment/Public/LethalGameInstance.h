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
	UPROPERTY(EditDefaultsOnly, DisplayName = "Default Mouse Sensitivity", Category = "Config", meta = (ClampMin = 1.f, ClampMax = 100.f))
	float Default_MouseSens;

	/** Player default mouse sensitivity while Aiming */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Default Mouse Aiming Sensitivity", Category = "Config", meta = (ClampMin = 1.f, ClampMax = 100.f))
	float Default_ADS_MouseSens;

	/** Player default controller sensitivity */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Default Controller Sensitivity", Category = "Config", meta = (ClampMin = 1.f, ClampMax = 100.f))
	float Default_ConSens;

	/** Player default mouse sensitivity while Aiming */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Default Controller Aiming Sensitivity", Category = "Config", meta = (ClampMin = 1.f, ClampMax = 100.f))
	float Default_ADS_ConSens;

	/** Defaults */
	FDefaultUserSettings()
	{
		Default_MouseSens = 50;
		Default_ADS_MouseSens = 50;
		Default_ConSens = 50;
		Default_ADS_ConSens = 50;
	}
};

/** On User Settings Updated Event */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUserSettingsUpdateDelegate, float, AimSens_Mouse, float, ADS_MouseSens, float, AimSens_Controller, float, ADS_ConSens);

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
	UPROPERTY(EditDefaultsOnly, DisplayName = "User Settings", Category = "Config")
	FDefaultUserSettings DefaultUserSettings;

	/** Current sensitivity for mouse */
	float Current_MouseSens;

	/** Current sensitivity for mouse while aiming */
	float Current_ADS_MouseSens;

	/** Current sensitivity for controller */
	float Current_ControllerSens;

	/** Current sensitivity for controller while aiming */
	float Current_ADS_ControllerSens;

public:
	ULethalGameInstance(const FObjectInitializer & ObjectInitializer);


	////////////////////////////////////////////////////////////////////////////////
	// Delegates

	/** Broadcasts new values to User */
	FOnUserSettingsUpdateDelegate OnUserSettingsUpdate;


	////////////////////////////////////////////////////////////////////////////////
	// Display widget request functions

	UFUNCTION(BlueprintCallable)
	void DisplayMainMenu();

	void ToggleInGameMenu();

	void DisplayGameOverMenu();


	////////////////////////////////////////////////////////////////////////////////
	// User Settings functions (Accessed through Menu interface and other classes)

	virtual void GetCurrentUserValues(float & Mouse_Sensitivity, float & Mouse_ADS_Sensitivity, float & Controller_Sensitivity, float & Controller_ADS_Sensitivity) override;

protected:
	virtual void Init() override;

	////////////////////////////////////////////////////////////////////////////////
	// Game action functions (Accessed through Menu interface)

	virtual void PlayGame() override;

	virtual void ReturnToMainMenu() override;

	virtual void OuitGame() override;

	virtual void ResumeGame() override;

	virtual void RestartGame() override;


	////////////////////////////////////////////////////////////////////////////////
	// User Settings functions (Accessed through Menu interface)

	virtual void SetNewUserSettings(float Mouse_Sensitivity, float Mouse_ADS_Sensitivity, float Controller_Sensitivity, float Controller_ADS_Sensitivity) override;

	virtual void GetDefaultUserValues(float & Mouse_Sensitivity, float & Mouse_ADS_Sensitivity, float & Controller_Sensitivity, float & Controller_ADS_Sensitivity) override;

private:
	////////////////////////////////////////////////////////////////////////////////
	// Load widget functions
	
	void LoadInGameMenu();

	void LoadGameOverMenu();

	void LoadMainMenu();

	void PauseGame();
	
};
