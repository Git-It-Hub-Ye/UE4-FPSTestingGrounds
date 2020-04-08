// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "ControlsWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "InGameMenuWidget.h"

bool UInGameMenuWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (Button_Resume)
	{
		Button_Resume->OnClicked.AddDynamic(this, &UInGameMenuWidget::ResumeGame);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Resume is missing from In Game Menu Widget")) return false; }

	if (Button_Controls)
	{
		Button_Controls->OnClicked.AddDynamic(this, &UInGameMenuWidget::ViewControls);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Controls is missing from In Game Menu Widget")) }

	if (Button_Restart)
	{
		Button_Restart->OnClicked.AddDynamic(this, &UInGameMenuWidget::WantsToRestart);

		if (Button_ConfirmRestart)
		{
			Button_ConfirmRestart->OnClicked.AddDynamic(this, &UInGameMenuWidget::RestartGame);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmRestart is missing from In Game Menu Widget")) }

		if (Button_CancelRestart)
		{
			Button_CancelRestart->OnClicked.AddDynamic(this, &UInGameMenuWidget::ReturnToInGameMenu);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelRestart is missing from In Game Menu Widget")) return false; }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Restart is missing from In Game Menu Widget")) }

	if (Button_MainMenu)
	{
		Button_MainMenu->OnClicked.AddDynamic(this, &UInGameMenuWidget::WantsToReturn);

		if (Button_ConfirmReturn)
		{
			Button_ConfirmReturn->OnClicked.AddDynamic(this, &UInGameMenuWidget::ReturnToMainMenu);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmReturn is missing from In Game Menu Widget")) }

		if (Button_CancelReturn)
		{
			Button_CancelReturn->OnClicked.AddDynamic(this, &UInGameMenuWidget::ReturnToInGameMenu);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelReturn is missing from In Game Menu Widget")) return false; }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_MainMenu is missing from In Game Menu Widget")) }

	return true;
}

void UInGameMenuWidget::NativePreConstruct()
{
	if (!ControlsPanel) { UE_LOG(LogTemp, Warning, TEXT("Control panel widget missing from In Game Menu Widget")) return; }

	ControlsPanel->SetWidgetInterface(this);
}

void UInGameMenuWidget::RequestReturnToParentWidget()
{
	ReturnToInGameMenu();
}

void UInGameMenuWidget::ResumeGame()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for InGameMenu Widget")) return; }
	MenuInterface->PauseGame();
	TearDown();
}

void UInGameMenuWidget::ViewControls()
{
	if (!WidgetSwitcher || !ControlsPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to ControlsPanel within In Game Menu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(ControlsPanel);
}

void UInGameMenuWidget::WantsToRestart()
{
	if (!WidgetSwitcher || !RestartPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to RestartPanel within In Game Menu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(RestartPanel);
}

void UInGameMenuWidget::RestartGame()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for InGameMenu Widget")) return; }
	MenuInterface->RestartGame();
}

void UInGameMenuWidget::WantsToReturn()
{
	if (!WidgetSwitcher || !ReturnMenuPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to ReturnMenuPanel within In Game Menu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(ReturnMenuPanel);
}

void UInGameMenuWidget::ReturnToMainMenu()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for InGameMenu Widget")) return; }
	MenuInterface->ReturnToMainMenu();
}

void UInGameMenuWidget::ReturnToInGameMenu()
{
	if (!WidgetSwitcher || !PausePanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to PausePanel within In Game Menu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(PausePanel);
}

