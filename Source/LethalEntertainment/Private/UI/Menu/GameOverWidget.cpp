// Copyright 2018 - 2021 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "MenuButtonsWidget.h"
#include "GameOverWidget.h"

bool UGameOverWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (Button_Restart && Button_Restart->GetButton())
	{
		Button_Restart->GetButton()->OnClicked.AddDynamic(this, &UGameOverWidget::RestartGame);
		Button_Restart->GetButton()->OnHovered.AddDynamic(this, &UGameOverWidget::ButtonRestartOnHover);
		Button_Restart->OnWidgetFocused.AddUniqueDynamic(this, &UGameOverWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Restart is missing from GameOverMenu Widget")) return false; }

	if (Button_MainMenu && Button_MainMenu->GetButton())
	{
		Button_MainMenu->GetButton()->OnClicked.AddDynamic(this, &UGameOverWidget::WantsToReturn);
		Button_MainMenu->GetButton()->OnHovered.AddDynamic(this, &UGameOverWidget::ButtonMainMenuOnHover);
		Button_MainMenu->OnWidgetFocused.AddUniqueDynamic(this, &UGameOverWidget::SetCurrentFocusedWidgetName);

		if (Button_ConfirmReturn && Button_ConfirmReturn->GetButton())
		{
			Button_ConfirmReturn->GetButton()->OnClicked.AddDynamic(this, &UGameOverWidget::ReturnToMainMenu);
			Button_ConfirmReturn->GetButton()->OnHovered.AddDynamic(this, &UGameOverWidget::ButtonConfirmReturnOnHover);
			Button_ConfirmReturn->OnWidgetFocused.AddUniqueDynamic(this, &UGameOverWidget::SetCurrentFocusedWidgetName);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmReturn is missing from GameOverMenu Widget")) return false; }

		if (Button_CancelReturn && Button_CancelReturn->GetButton())
		{
			Button_CancelReturn->GetButton()->OnClicked.AddDynamic(this, &UGameOverWidget::CancelReturn);
			Button_CancelReturn->GetButton()->OnHovered.AddDynamic(this, &UGameOverWidget::ButtonCancelReturnOnHover);
			Button_CancelReturn->OnWidgetFocused.AddUniqueDynamic(this, &UGameOverWidget::SetCurrentFocusedWidgetName);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelReturn is missing from GameOverMenu Widget")) return false; }

	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_MainMenu is missing from GameOverMenu Widget")) return false; }

	return true;
}

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!Button_Restart) { UE_LOG(LogTemp, Warning, TEXT("Button_Restart is missing from GameOverMenu Widget")) return; }
	Button_Restart->SetFocusToButton();
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UGameOverWidget::RestartGame()
{
	Button_Restart->PlayPressedSound();

	if (WidgetSwitcher && LoadingScreen)
	{
		WidgetSwitcher->SetActiveWidget(LoadingScreen);
	}

	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for GameOverMenu Widget")) return; }
	MenuInterface->RestartGame();
}

void UGameOverWidget::WantsToReturn()
{
	Button_MainMenu->PlayPressedSound();

	if (!WidgetSwitcher || !ReturnMenuPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to ReturnMenuPanel within GameOverMenu Widget")) return; }
	Name_LastButton = *Button_MainMenu->GetName();
	WidgetSwitcher->SetActiveWidget(ReturnMenuPanel);

	if (!Button_CancelReturn) { UE_LOG(LogTemp, Warning, TEXT("Button_CancelReturn is missing from GameOverMenu Widget")) return; }
	Button_CancelReturn->SetFocusToButton();
}

void UGameOverWidget::CancelReturn()
{
	Button_CancelReturn->PlayPressedSound();

	if (!WidgetSwitcher || !MenuPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to MenuPanel within GameOverMenu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(MenuPanel);
	SetWidgetToFocus(Name_LastButton);
}

void UGameOverWidget::ReturnToMainMenu()
{
	Button_ConfirmReturn->PlayPressedSound();

	if (WidgetSwitcher && LoadingScreen)
	{
		WidgetSwitcher->SetActiveWidget(LoadingScreen);
	}

	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for GameOverMenu Widget")) return; }
	MenuInterface->ReturnToMainMenu();
}


////////////////////////////////////////////////////////////////////////////////
// Button Hover

void UGameOverWidget::ButtonRestartOnHover()
{
	Button_Restart->SetFocusToButton();
}

void UGameOverWidget::ButtonMainMenuOnHover()
{
	Button_MainMenu->SetFocusToButton();
}

void UGameOverWidget::ButtonConfirmReturnOnHover()
{
	Button_ConfirmReturn->SetFocusToButton();
}

void UGameOverWidget::ButtonCancelReturnOnHover()
{
	Button_CancelReturn->SetFocusToButton();
}


////////////////////////////////////////////////////////////////////////////////
// Inputs

void UGameOverWidget::BackInput()
{
	CancelReturn();
}

