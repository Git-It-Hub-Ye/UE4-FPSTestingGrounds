// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Menu/MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "LethalGameInstance.h"

ULethalGameInstance::ULethalGameInstance(const FObjectInitializer & ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultMainMenuWidget(TEXT("/Game/Dynamic/UI/WBP_MainMenu"));
	if (DefaultMainMenuWidget.Class)
	{
		MainMenuWidget = DefaultMainMenuWidget.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultInGameMenuWidget(TEXT("/Game/Dynamic/UI/WBP_InGameMenu"));
	if (DefaultInGameMenuWidget.Class)
	{
		InGameMenuWidget = DefaultInGameMenuWidget.Class;
	}
}

void ULethalGameInstance::LoadMenu()
{
	if (!MainMenuWidget) { UE_LOG(LogTemp, Warning, TEXT("Cant't find Default Main Menu in Game Instance")) return; }
	UMenuWidget * Menu = CreateWidget<UMenuWidget>(this, MainMenuWidget);

	if (!Menu) { UE_LOG(LogTemp, Warning, TEXT("Cant't find Main Menu Widget in Game Instance")) return; }
	// Instance implements menu interface
	Menu->SetMenuInterface(this);
	Menu->Setup();
}

void ULethalGameInstance::LoadInGameMenu()
{
	if (!InGameMenuWidget) { UE_LOG(LogTemp, Warning, TEXT("Cant't find Default In Game Menu in Game Instance")) return; }
	InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuWidget);

	if (!InGameMenu) { UE_LOG(LogTemp, Warning, TEXT("Cant't find In Game Menu Widget in Game Instance")) return; }
	// Instance implements menu interface
	InGameMenu->SetMenuInterface(this);
	PauseGame();
	InGameMenu->Setup();
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

void ULethalGameInstance::PauseGame()
{
	if (!GetWorld()) { UE_LOG(LogTemp, Warning, TEXT("Can't find World in Game Instance")) return; }
	GetWorld()->IsPaused() ? UGameplayStatics::SetGamePaused(GetWorld(), false) : UGameplayStatics::SetGamePaused(GetWorld(), true);
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
