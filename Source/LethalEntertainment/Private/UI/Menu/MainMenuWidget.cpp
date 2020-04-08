// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "ControlsWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "MainMenuWidget.h"

bool UMainMenuWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }
	if (Button_Play)
	{
		Button_Play->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayGame);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Play is missing from Main Menu Widget")) return false; }

	if (Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UMainMenuWidget::WantsToQuit);

		if (Button_ConfirmQuit)
		{
			Button_ConfirmQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmQuit is missing from Main Menu Widget")) return false; }

		if (Button_CancelQuit)
		{
			Button_CancelQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::ReturnToMainMenu);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelQuit is missing from Main Menu Widget")) return false; }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Quit is missing from Main Menu Widget")) return false; }

	if (Button_Controls)
	{
		Button_Controls->OnClicked.AddDynamic(this, &UMainMenuWidget::ViewControls);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Controls is missing from Main Menu Widget")) }

	return true;
}

void UMainMenuWidget::NativePreConstruct()
{
	if (!ControlsPanel) { UE_LOG(LogTemp, Warning, TEXT("Control panel widget missing from Main Menu Widget")) return; }

	ControlsPanel->SetWidgetInterface(this);
}

void UMainMenuWidget::RequestReturnToParentWidget()
{
	ReturnToMainMenu();
}

void UMainMenuWidget::PlayGame()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for MainMenu Widget")) return; }
	MenuInterface->PlayGame();
}

void UMainMenuWidget::WantsToQuit()
{
	if (!WidgetSwitcher || !QuitMenu) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to QuitMenu in Main Menu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(QuitMenu);
}

void UMainMenuWidget::QuitGame()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for MainMenu Widget")) return; }
	MenuInterface->OuitGame();
}

void UMainMenuWidget::ViewControls()
{
	if (!WidgetSwitcher || !ControlsPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to ControlsPanel in Main Menu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(ControlsPanel);
}

void UMainMenuWidget::ReturnToMainMenu()
{
	if (!WidgetSwitcher || !MainMenu) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to MainMenu in Main Menu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(MainMenu);
}

