// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Menu/MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LethalSaveGame.h"
#include "LethalGameInstance.h"

ULethalGameInstance::ULethalGameInstance(const FObjectInitializer & ObjectInitializer)
{
	SaveGameClass = ULethalSaveGame::StaticClass();

	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultMainMenuWidget(TEXT("/Game/Dynamic/UI/Menus/WBP_MainMenu"));
	if (DefaultMainMenuWidget.Class)
	{
		MainMenuWidget = DefaultMainMenuWidget.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultInGameMenuWidget(TEXT("/Game/Dynamic/UI/Menus/WBP_InGameMenu"));
	if (DefaultInGameMenuWidget.Class)
	{
		InGameMenuWidget = DefaultInGameMenuWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultGameOverWidget(TEXT("/Game/Dynamic/UI/Menus/WBP_GameOverMenu"));
	if (DefaultGameOverWidget.Class)
	{
		GameOverWidget = DefaultGameOverWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<ULethalSaveGame> DefaultSaveGameClass(TEXT("/Game/Dynamic/Blueprints/BP_SaveGame"));
	if (DefaultSaveGameClass.Class)
	{
		SaveGameClass = DefaultSaveGameClass.Class;
	}
}

void ULethalGameInstance::Init()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlot1, 0)) 
	{ 
		LoadGameData();
	}
	else
	{
		Current_MouseSens = DefaultUserSettings.Default_MouseSens;
		Current_ADS_MouseSens = DefaultUserSettings.Default_ADS_MouseSens;
		Current_ControllerSens = DefaultUserSettings.Default_ConSens;
		Current_ADS_ControllerSens = DefaultUserSettings.Default_ADS_ConSens;
		Current_InvertY = DefaultUserSettings.Default_InvertY;
		Current_Vibration = DefaultUserSettings.Default_Vibration;
		Current_ControlType = EControlType::MouseAndKeyboard;
		CreateSaveGameData();
	}
}


////////////////////////////////////////////////////////////////////////////////
// Display widget request functions

void ULethalGameInstance::DisplayMainMenu()
{
	LoadMainMenu();
}

void ULethalGameInstance::ToggleInGameMenu()
{
	if (InGameMenu && InGameMenu->IsValidLowLevel() && InGameMenu->IsInViewport())
	{
		InGameMenu->TearDown();
		PauseGame();
	}
	else
	{
		LoadInGameMenu();
	}
}

void ULethalGameInstance::DisplayGameOverMenu()
{
	LoadGameOverMenu();
}


////////////////////////////////////////////////////////////////////////////////
// Load widget functions

void ULethalGameInstance::LoadMainMenu()
{
	if (!MainMenuWidget) { UE_LOG(LogTemp, Warning, TEXT("Cant't find Default MainMenu in Game Instance")) return; }
	UMenuWidget * Menu = CreateWidget<UMenuWidget>(this, MainMenuWidget);

	if (!Menu) { UE_LOG(LogTemp, Warning, TEXT("Cant't find MainMenu Widget in Game Instance")) return; }
	// Instance implements menu interface
	Menu->SetMenuInterface(this);
	Menu->Setup();
	Menu->SetInitialControlType(Current_ControlType);
}

void ULethalGameInstance::LoadInGameMenu()
{
	if (!InGameMenuWidget) { UE_LOG(LogTemp, Warning, TEXT("Cant't find Default InGameMenu in Game Instance")) return; }
	InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuWidget);

	if (!InGameMenu) { UE_LOG(LogTemp, Warning, TEXT("Cant't find InGameMen Widget in Game Instance")) return; }
	// Instance implements menu interface
	InGameMenu->SetMenuInterface(this);
	PauseGame();
	InGameMenu->Setup();
	InGameMenu->SetInitialControlType(Current_ControlType);
}

void ULethalGameInstance::LoadGameOverMenu()
{
	if (!GameOverWidget) { UE_LOG(LogTemp, Warning, TEXT("Cant't find Default GameOverMenu in Game Instance")) return; }
	UMenuWidget * Menu = CreateWidget<UMenuWidget>(this, GameOverWidget);

	if (!Menu) { UE_LOG(LogTemp, Warning, TEXT("Cant't find GameOverMenu in Game Instance")) return; }
	// Instance implements menu interface
	Menu->SetMenuInterface(this);
	Menu->Setup();
}

void ULethalGameInstance::PauseGame()
{
	if (!GetWorld()) { UE_LOG(LogTemp, Warning, TEXT("Can't find World in Game Instance")) return; }
	GetWorld()->IsPaused() ? UGameplayStatics::SetGamePaused(GetWorld(), false) : UGameplayStatics::SetGamePaused(GetWorld(), true);
}


////////////////////////////////////////////////////////////////////////////////
// Game action functions (Accessed through Menu interface)

void ULethalGameInstance::ResumeGame()
{
	ToggleInGameMenu();
}

void ULethalGameInstance::PlayGame()
{
	if (!GetWorld()) { UE_LOG(LogTemp, Warning, TEXT("Can't find World in Game Instance")) return; }
	UGameplayStatics::OpenLevel(GetWorld(), "/Game/Dynamic/Levels/TileMap");
}

void ULethalGameInstance::RestartGame()
{
	// Restart Game only works with childs of GameMode not GameModeBase - this way works also.
	PlayGame();
}

void ULethalGameInstance::ReturnToMainMenu()
{
	if (!GetWorld()) { UE_LOG(LogTemp, Warning, TEXT("Can't find World in Game Instance")) return; }
	UGameplayStatics::OpenLevel(GetWorld(), "/Game/Dynamic/Levels/MainMenu");
}

void ULethalGameInstance::OuitGame()
{
	if (!GetWorld()) { UE_LOG(LogTemp, Warning, TEXT("Can't find World in Game Instance")) return; }
	UKismetSystemLibrary::QuitGame(GetWorld(), GetFirstLocalPlayerController(), EQuitPreference::Quit);
}

void ULethalGameInstance::SetNewUserSettings(float Mouse_Sensitivity, float Mouse_ADS_Sensitivity, float Controller_Sensitivity, float Controller_ADS_Sensitivity, bool Invert_Y, bool Vibration)
{
	Current_MouseSens = Mouse_Sensitivity;
	Current_ADS_MouseSens = Mouse_ADS_Sensitivity;
	Current_ControllerSens = Controller_Sensitivity;
	Current_ADS_ControllerSens = Controller_ADS_Sensitivity;
	Current_InvertY = Invert_Y;
	Current_Vibration = Vibration;
	OnUserSettingsUpdate.Broadcast(Current_MouseSens, Current_ADS_MouseSens, Current_ControllerSens, Current_ADS_ControllerSens, Current_InvertY, Current_Vibration);
	SaveUserSettingsData();
}

void ULethalGameInstance::GetCurrentUserValues(float & Mouse_Sensitivity, float & Mouse_ADS_Sensitivity, float & Controller_Sensitivity, float & Controller_ADS_Sensitivity, bool & Invert_Y, bool & Vibration)
{
	Mouse_Sensitivity = Current_MouseSens;
	Mouse_ADS_Sensitivity = Current_ADS_MouseSens;
	Controller_Sensitivity = Current_ControllerSens;
	Controller_ADS_Sensitivity = Current_ADS_ControllerSens;
	Invert_Y = Current_InvertY;
	Vibration = Current_Vibration;
}

void ULethalGameInstance::GetDefaultUserValues(float & Mouse_Sensitivity, float & Mouse_ADS_Sensitivity, float & Controller_Sensitivity, float & Controller_ADS_Sensitivity, bool & Invert_Y, bool & Vibration)
{
	Mouse_Sensitivity = DefaultUserSettings.Default_MouseSens;
	Mouse_ADS_Sensitivity = DefaultUserSettings.Default_ADS_MouseSens;
	Controller_Sensitivity = DefaultUserSettings.Default_ConSens;
	Controller_ADS_Sensitivity = DefaultUserSettings.Default_ADS_ConSens;
	Invert_Y = DefaultUserSettings.Default_InvertY;
	Vibration = DefaultUserSettings.Default_Vibration;
}

void ULethalGameInstance::SetNewControlType(EControlType NewControlType)
{
	Current_ControlType = NewControlType;
	SaveControlTypeData();
}


////////////////////////////////////////////////////////////////////////////////
// Save

void ULethalGameInstance::LoadGameData()
{
	SaveGameRef = Cast<ULethalSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot1, 0));
	if (SaveGameRef)
	{
		Current_MouseSens = SaveGameRef->Saved_MouseSens;
		Current_ADS_MouseSens = SaveGameRef->Saved_ADS_MouseSens;
		Current_ControllerSens = SaveGameRef->Saved_ConSens;
		Current_ADS_ControllerSens = SaveGameRef->Saved_ADS_ConSens;
		Current_InvertY = SaveGameRef->Saved_InvertY;
		Current_Vibration = SaveGameRef->Saved_Vibration;
		Current_ControlType = SaveGameRef->Saved_ControlType;
	}
}

void ULethalGameInstance::SaveUserSettingsData()
{
	if (!SaveGameRef->IsValidLowLevel())
	{
		CreateSaveGameData();
	}
	else if (SaveGameRef->IsValidLowLevel())
	{
		SaveGameRef->Saved_InvertY = Current_InvertY;
		SaveGameRef->Saved_Vibration = Current_Vibration;
		SaveGameRef->Saved_MouseSens = Current_MouseSens;
		SaveGameRef->Saved_ADS_MouseSens = Current_ADS_MouseSens;
		SaveGameRef->Saved_ConSens = Current_ControllerSens;
		SaveGameRef->Saved_ADS_ConSens = Current_ADS_ControllerSens;
		UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveSlot1, 0);
	}
}

void ULethalGameInstance::SaveControlTypeData()
{
	if (!SaveGameRef->IsValidLowLevel())
	{
		CreateSaveGameData();
	}
	else if (SaveGameRef->IsValidLowLevel())
	{
		SaveGameRef->Saved_ControlType = Current_ControlType;
		UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveSlot1, 0);
	}
}

void ULethalGameInstance::CreateSaveGameData()
{
	if (SaveGameClass)
	{
		SaveGameRef = Cast<ULethalSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
		if (SaveGameRef)
		{
			UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveSlot1, 0);
		}
	}
}

