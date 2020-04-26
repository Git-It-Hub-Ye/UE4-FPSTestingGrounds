// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "ControlsWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "MenuButtonsWidget.h"
#include "InGameMenuWidget.h"

bool UInGameMenuWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (Button_Resume && Button_Resume->GetButton())
	{
		Button_Resume->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::ResumeGame);
		Button_Resume->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonResumeOnHover);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Resume is missing from InGameMenu Widget")) return false; }

	if (Button_Controls && Button_Controls->GetButton())
	{
		Button_Controls->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::ViewControls);
		Button_Controls->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonControlsOnHover);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Controls is missing from InGameMenu Widget")) }

	if (Button_Restart && Button_Restart->GetButton())
	{
		Button_Restart->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::WantsToRestart);
		Button_Restart->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonRestartOnHover);

		if (Button_ConfirmRestart && Button_ConfirmRestart->GetButton())
		{
			Button_ConfirmRestart->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::RestartGame);
			Button_ConfirmRestart->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonConfirmRestartOnHover);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmRestart is missing from InGameMenu Widget")) }

		if (Button_CancelRestart && Button_CancelRestart->GetButton())
		{
			Button_CancelRestart->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::ReturnToInGameMenu);
			Button_CancelRestart->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonCancelRestartOnHover);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelRestart is missing from InGameMenu Widget")) return false; }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Restart is missing from InGameMenu Widget")) }

	if (Button_MainMenu && Button_MainMenu->GetButton())
	{
		Button_MainMenu->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::WantsToReturn);
		Button_MainMenu->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonMainMenuOnHover);

		if (Button_ConfirmReturn && Button_ConfirmReturn->GetButton())
		{
			Button_ConfirmReturn->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::ReturnToMainMenu);
			Button_ConfirmReturn->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonConfirmReturnOnHover);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmReturn is missing from InGameMenu Widget")) return false; }

		if (Button_CancelReturn && Button_CancelReturn->GetButton())
		{
			Button_CancelReturn->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::ReturnToInGameMenu);
			Button_CancelReturn->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonCancelReturnOnHover);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelReturn is missing from InGameMenu Widget")) return false; }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_MainMenu is missing from InGameMenu Widget")) return false; }

	return true;
}

void UInGameMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (!ControlsPanel){ UE_LOG(LogTemp, Warning, TEXT("ControlsPpanel widget missing from InGameMenu Widget")) return; }
	ControlsPanel->SetWidgetInterface(this);
}

void UInGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!Button_Resume && Button_Resume->GetButton()) { UE_LOG(LogTemp, Warning, TEXT("Button_Resume is missing from InGameMenu Widget")) return; }
	Button_Resume->SetFocusToButton();
}

void UInGameMenuWidget::RequestReturnToParentWidget()
{
	ReturnToInGameMenu();
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UInGameMenuWidget::ResumeGame()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for InGameMenu Widget")) return; }
	MenuInterface->PauseGame();
	TearDown();
}

void UInGameMenuWidget::ViewControls()
{
	if (!WidgetSwitcher || !ControlsPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to ControlsPanel within InGameMenu Widget")) return; }
	Name_LastButton = *Button_Controls->GetName();
	WidgetSwitcher->SetActiveWidget(ControlsPanel);
	ControlsPanel->SetFocus();
}

void UInGameMenuWidget::WantsToRestart()
{
	if (!WidgetSwitcher || !RestartPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to RestartPanel within InGameMenu Widget")) return; }
	Name_LastButton = *Button_Restart->GetName();
	WidgetSwitcher->SetActiveWidget(RestartPanel);

	if (!Button_CancelRestart) { UE_LOG(LogTemp, Warning, TEXT("Button_CancelRestart is missing from InGameMenu Widget")) return; }
	Button_CancelRestart->SetFocusToButton();
}

void UInGameMenuWidget::RestartGame()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for InGameMenu Widget")) return; }
	MenuInterface->RestartGame();
}

void UInGameMenuWidget::WantsToReturn()
{
	if (!WidgetSwitcher || !ReturnMenuPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to ReturnMenuPanel within InGameMenu Widget")) return; }
	Name_LastButton = *Button_MainMenu->GetName();
	WidgetSwitcher->SetActiveWidget(ReturnMenuPanel);

	if (!Button_CancelReturn) { UE_LOG(LogTemp, Warning, TEXT("Button_CancelReturn is missing from InGameMenu Widget")) return; }
	Button_CancelReturn->SetFocusToButton();
}

void UInGameMenuWidget::ReturnToMainMenu()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for InGameMenu Widget")) return; }
	MenuInterface->ReturnToMainMenu();
}

void UInGameMenuWidget::ReturnToInGameMenu()
{
	if (!WidgetSwitcher || !PausePanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to PausePanel within InGameMenu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(PausePanel);
	SetWidgetToFocus(Name_LastButton);
}


////////////////////////////////////////////////////////////////////////////////
// Button Hover

void UInGameMenuWidget::ButtonResumeOnHover()
{
	Button_Resume->SetFocusToButton();
}

void UInGameMenuWidget::ButtonControlsOnHover()
{
	Button_Controls->SetFocusToButton();
}

void UInGameMenuWidget::ButtonRestartOnHover()
{
	Button_Restart->SetFocusToButton();
}

void UInGameMenuWidget::ButtonMainMenuOnHover()
{
	Button_MainMenu->SetFocusToButton();
}

void UInGameMenuWidget::ButtonConfirmRestartOnHover()
{
	Button_ConfirmRestart->SetFocusToButton();
}

void UInGameMenuWidget::ButtonCancelRestartOnHover()
{
	Button_CancelRestart->SetFocusToButton();
}

void UInGameMenuWidget::ButtonConfirmReturnOnHover()
{
	Button_ConfirmReturn->SetFocusToButton();
}

void UInGameMenuWidget::ButtonCancelReturnOnHover()
{
	Button_CancelReturn->SetFocusToButton();
}

