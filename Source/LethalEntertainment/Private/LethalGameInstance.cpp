// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Menu/MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LethalGameInstance.h"

ULethalGameInstance::ULethalGameInstance(const FObjectInitializer & ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultMainMenuWidget(TEXT("/Game/Dynamic/UI/WBP_MainMenu"));
	if (DefaultMainMenuWidget.Class)
	{
		MainMenuWidget = DefaultMainMenuWidget.Class;
	}
}

void ULethalGameInstance::LoadMenu()
{
	if (!MainMenuWidget) { return; }
	UMenuWidget * Menu = CreateWidget<UMenuWidget>(this, MainMenuWidget);

	if (!Menu) { return; }
	// Instance implements menu interface
	Menu->SetMenuInterface(this);
	Menu->Setup();
}

void ULethalGameInstance::CleanupSessionOnReturnToMenu()
{
}

void ULethalGameInstance::PlayGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), "/Game/Dynamic/Levels/TileMap");
}

void ULethalGameInstance::OuitGame()
{
}
