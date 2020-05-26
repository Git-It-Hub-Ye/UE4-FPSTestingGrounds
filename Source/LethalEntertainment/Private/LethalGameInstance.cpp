// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Menu/MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "LethalGameInstance.h"

ULethalGameInstance::ULethalGameInstance(const FObjectInitializer & ObjectInitializer)
{
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
}

void ULethalGameInstance::Init()
{
	Current_MouseSens = DefaultUserSettings.Default_MouseSens;
	Current_ADS_MouseSens = DefaultUserSettings.Default_ADS_MouseSens;
	Current_ControllerSens = DefaultUserSettings.Default_ConSens;
	Current_ADS_ControllerSens = DefaultUserSettings.Default_ADS_ConSens;
	Current_InvertY = DefaultUserSettings.Default_InvertY;
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

void ULethalGameInstance::SetNewUserSettings(float Mouse_Sensitivity, float Mouse_ADS_Sensitivity, float Controller_Sensitivity, float Controller_ADS_Sensitivity, bool Invert_Y)
{
	Current_MouseSens = Mouse_Sensitivity;
	Current_ADS_MouseSens = Mouse_ADS_Sensitivity;
	Current_ControllerSens = Controller_Sensitivity;
	Current_ADS_ControllerSens = Controller_ADS_Sensitivity;
	Current_InvertY = Invert_Y;
	OnUserSettingsUpdate.Broadcast(Current_MouseSens, Current_ADS_MouseSens, Current_ControllerSens, Current_ADS_ControllerSens, Current_InvertY);
}

void ULethalGameInstance::GetCurrentUserValues(float & Mouse_Sensitivity, float & Mouse_ADS_Sensitivity, float & Controller_Sensitivity, float & Controller_ADS_Sensitivity, bool & Invert_Y)
{
	Mouse_Sensitivity = Current_MouseSens;
	Mouse_ADS_Sensitivity = Current_ADS_MouseSens;
	Controller_Sensitivity = Current_ControllerSens;
	Controller_ADS_Sensitivity = Current_ADS_ControllerSens;
	Invert_Y = Current_InvertY;
}

void ULethalGameInstance::GetDefaultUserValues(float & Mouse_Sensitivity, float & Mouse_ADS_Sensitivity, float & Controller_Sensitivity, float & Controller_ADS_Sensitivity, bool & Invert_Y)
{
	Mouse_Sensitivity = DefaultUserSettings.Default_MouseSens;
	Mouse_ADS_Sensitivity = DefaultUserSettings.Default_ADS_MouseSens;
	Controller_Sensitivity = DefaultUserSettings.Default_ConSens;
	Controller_ADS_Sensitivity = DefaultUserSettings.Default_ADS_ConSens;
	Invert_Y = DefaultUserSettings.Default_InvertY;
}

