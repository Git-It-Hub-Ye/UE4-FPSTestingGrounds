// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "GameOverWidget.h"

bool UGameOverWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (Button_Restart)
	{
		Button_Restart->OnClicked.AddDynamic(this, &UGameOverWidget::RestartGame);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Restart is missing from GameOverMenu Widget")) return false; }

	if (Button_MainMenu)
	{
		Button_MainMenu->OnClicked.AddDynamic(this, &UGameOverWidget::WantsToReturn);

		if (Button_ConfirmReturn)
		{
			Button_ConfirmReturn->OnClicked.AddDynamic(this, &UGameOverWidget::ReturnToMainMenu);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmReturn is missing from GameOverMenu Widget")) return false; }

		if (Button_CancelReturn)
		{
			Button_CancelReturn->OnClicked.AddDynamic(this, &UGameOverWidget::CancelReturn);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelReturn is missing from GameOverMenu Widget")) return false; }

	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_MainMenu is missing from GameOverMenu Widget")) return false; }

	return true;
}

void UGameOverWidget::RestartGame()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for GameOverMenu Widget")) return; }
	MenuInterface->RestartGame();
}

void UGameOverWidget::WantsToReturn()
{
	if (!WidgetSwitcher || !ReturnMenuPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to ReturnMenuPanel within GameOverMenu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(ReturnMenuPanel);
}

void UGameOverWidget::CancelReturn()
{
	if (!WidgetSwitcher || !ReturnMenuPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to MenuPanel within GameOverMenu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(MenuPanel);
}

void UGameOverWidget::ReturnToMainMenu()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for GameOverMenu Widget")) return; }
	MenuInterface->ReturnToMainMenu();
}
