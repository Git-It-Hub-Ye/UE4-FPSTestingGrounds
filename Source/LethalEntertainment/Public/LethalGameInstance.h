// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "LethalGameInstance.generated.h"

class UUserWidget;
class UMenuWidget;
class ULethalSaveGame;
enum class EControlType : uint8;

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

	/** Player default controller sensitivity while Aiming */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Default Controller Aiming Sensitivity", Category = "Config", meta = (ClampMin = 1.f, ClampMax = 100.f))
	float Default_ADS_ConSens;

	/** Player default Invert Y Axis value */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Default Invert Y Axis", Category = "Config")
	bool Default_InvertY;

	/** Player default Invert Y Axis value */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Default Invert Y Axis", Category = "Config")
	bool Default_Vibration;

	/** Player default control type */
	EControlType Default_ControlType;

	/** Defaults */
	FDefaultUserSettings()
	{
		Default_MouseSens = 50;
		Default_ADS_MouseSens = 50;
		Default_ConSens = 50;
		Default_ADS_ConSens = 50;
		Default_InvertY = false;
		Default_Vibration = true;
	}
};

/** On User Settings Updated Event */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnUserSettingsUpdateDelegate, float, AimSens_Mouse, float, ADS_MouseSens, float, AimSens_Controller, float, ADS_ConSens, bool, InvertY, bool, Vibration);

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

	/** Current value for Invert Y Axis */
	bool Current_InvertY;

	/** Current value for vibration */
	bool Current_Vibration;

	/** Current selected control type */
	EControlType Current_ControlType;


	////////////////////////////////////////////////////////////////////////////////
	// Save

	/** Save slot name */
	FString SaveSlot1 = FString(TEXT("SaveLot1"));

	/** Save class variable */
	UPROPERTY()
	ULethalSaveGame * SaveGameRef;

	/** The class for save game */
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<ULethalSaveGame> SaveGameClass;

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

	virtual void GetCurrentUserValues(float & Mouse_Sensitivity, float & Mouse_ADS_Sensitivity, float & Controller_Sensitivity, float & Controller_ADS_Sensitivity, bool & Invert_Y, bool & Vibration) override;

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

	virtual void SetNewUserSettings(float Mouse_Sensitivity, float Mouse_ADS_Sensitivity, float Controller_Sensitivity, float Controller_ADS_Sensitivity, bool Invert_Y, bool Vibration) override;

	virtual void GetDefaultUserValues(float & Mouse_Sensitivity, float & Mouse_ADS_Sensitivity, float & Controller_Sensitivity, float & Controller_ADS_Sensitivity, bool & Invert_Y, bool & Vibration) override;

	virtual void SetNewControlType(EControlType NewControlType) override;

private:
	////////////////////////////////////////////////////////////////////////////////
	// Load widget functions
	
	void LoadInGameMenu();

	void LoadGameOverMenu();

	void LoadMainMenu();

	void PauseGame();


	////////////////////////////////////////////////////////////////////////////////
	// Save

	/** Gets save game class */
	void LoadGameData();

	/** Save new user settings */
	void SaveUserSettingsData();

	/** Save control type */
	void SaveControlTypeData();

	/** Creates save game class */
	void CreateSaveGameData();
	
};
