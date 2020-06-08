// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "OptionsMenuWidget.h"
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
		Button_Resume->OnWidgetFocused.AddUniqueDynamic(this, &UInGameMenuWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Resume is missing from InGameMenu Widget")) return false; }

	if (Button_Options && Button_Options->GetButton())
	{
		Button_Options->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::ViewOptions);
		Button_Options->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonOptionsOnHover);
		Button_Options->OnWidgetFocused.AddUniqueDynamic(this, &UInGameMenuWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Options is missing from InGameMenu Widget")) }

	if (Button_Controls && Button_Controls->GetButton())
	{
		Button_Controls->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::ViewControls);
		Button_Controls->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonControlsOnHover);
		Button_Controls->OnWidgetFocused.AddUniqueDynamic(this, &UInGameMenuWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Controls is missing from InGameMenu Widget")) }

	if (Button_Restart && Button_Restart->GetButton())
	{
		Button_Restart->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::WantsToRestart);
		Button_Restart->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonRestartOnHover);
		Button_Restart->OnWidgetFocused.AddUniqueDynamic(this, &UInGameMenuWidget::SetCurrentFocusedWidgetName);

		if (Button_ConfirmRestart && Button_ConfirmRestart->GetButton())
		{
			Button_ConfirmRestart->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::ConfirmRestart);
			Button_ConfirmRestart->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonConfirmRestartOnHover);
			Button_ConfirmRestart->OnWidgetFocused.AddUniqueDynamic(this, &UInGameMenuWidget::SetCurrentFocusedWidgetName);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmRestart is missing from InGameMenu Widget")) }

		if (Button_CancelRestart && Button_CancelRestart->GetButton())
		{
			Button_CancelRestart->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::CancelRestart);
			Button_CancelRestart->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonCancelRestartOnHover);
			Button_CancelRestart->OnWidgetFocused.AddUniqueDynamic(this, &UInGameMenuWidget::SetCurrentFocusedWidgetName);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelRestart is missing from InGameMenu Widget")) return false; }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Restart is missing from InGameMenu Widget")) }

	if (Button_MainMenu && Button_MainMenu->GetButton())
	{
		Button_MainMenu->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::WantsToReturn);
		Button_MainMenu->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonMainMenuOnHover);
		Button_MainMenu->OnWidgetFocused.AddUniqueDynamic(this, &UInGameMenuWidget::SetCurrentFocusedWidgetName);

		if (Button_ConfirmReturn && Button_ConfirmReturn->GetButton())
		{
			Button_ConfirmReturn->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::ConfirmReturn);
			Button_ConfirmReturn->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonConfirmReturnOnHover);
			Button_ConfirmReturn->OnWidgetFocused.AddUniqueDynamic(this, &UInGameMenuWidget::SetCurrentFocusedWidgetName);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmReturn is missing from InGameMenu Widget")) return false; }

		if (Button_CancelReturn && Button_CancelReturn->GetButton())
		{
			Button_CancelReturn->GetButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::CancelReturn);
			Button_CancelReturn->GetButton()->OnHovered.AddDynamic(this, &UInGameMenuWidget::ButtonCancelReturnOnHover);
			Button_CancelReturn->OnWidgetFocused.AddUniqueDynamic(this, &UInGameMenuWidget::SetCurrentFocusedWidgetName);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelReturn is missing from InGameMenu Widget")) return false; }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_MainMenu is missing from InGameMenu Widget")) return false; }

	return true;
}

void UInGameMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (OptionsPanel)
	{
		OptionsPanel->SetMenuInterface(MenuInterface);
		OptionsPanel->SetUserWidgetInterface(this);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("OptionsPanel widget missing from InGameMenu Widget")) }

	if (!ControlsPanel){ UE_LOG(LogTemp, Warning, TEXT("ControlsPpanel widget missing from InGameMenu Widget")) return; }
	ControlsPanel->SetMenuInterface(MenuInterface);
	ControlsPanel->SetUserWidgetInterface(this);
	ControlsPanel->OnControlTypeSet.AddUniqueDynamic(this, &UInGameMenuWidget::SetControlType);
}

void UInGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_Resume && Button_Resume->GetButton())
	{
		Button_Resume->SetFocusToButton();
	}
	else{ UE_LOG(LogTemp, Warning, TEXT("Button_Resume is missing from InGameMenu Widget")) }
}

void UInGameMenuWidget::RequestReturnToParentWidget()
{
	ReturnToInGameMenu();
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UInGameMenuWidget::ResumeGame()
{
	Button_Resume->PlayPressedSound();

	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for InGameMenu Widget")) return; }
	MenuInterface->ResumeGame();
}

void UInGameMenuWidget::ViewOptions()
{
	Button_Options->PlayPressedSound();

	if (!WidgetSwitcher || !OptionsPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to OptionsPanel within InGameMenu Widget")) return; }
	Name_LastButton = *Button_Options->GetName();
	OptionsPanel->SetInitialValues();
	WidgetSwitcher->SetActiveWidget(OptionsPanel);
	OptionsPanel->SetFocus(Current_ControlType);
}

void UInGameMenuWidget::ViewControls()
{
	Button_Controls->PlayPressedSound();

	if (!WidgetSwitcher || !ControlsPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to ControlsPanel within InGameMenu Widget")) return; }
	Name_LastButton = *Button_Controls->GetName();
	WidgetSwitcher->SetActiveWidget(ControlsPanel);
	ControlsPanel->SetFocus();
	ControlsPanel->SetScrollBoxType(Current_ControlType);
}

void UInGameMenuWidget::WantsToRestart()
{
	Button_Restart->PlayPressedSound();

	if (!WidgetSwitcher || !RestartPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to RestartPanel within InGameMenu Widget")) return; }
	Name_LastButton = *Button_Restart->GetName();
	bIsAdditionalPanelOpen = true;
	WidgetSwitcher->SetActiveWidget(RestartPanel);

	if (!Button_CancelRestart) { UE_LOG(LogTemp, Warning, TEXT("Button_CancelRestart is missing from InGameMenu Widget")) return; }
	Button_CancelRestart->SetFocusToButton();
}

void UInGameMenuWidget::CancelRestart()
{
	Button_CancelRestart->PlayPressedSound();
	ReturnToInGameMenu();
}

void UInGameMenuWidget::ConfirmRestart()
{
	Button_ConfirmRestart->PlayPressedSound();

	if (WidgetSwitcher && LoadingScreen)
	{
		WidgetSwitcher->SetActiveWidget(LoadingScreen);
	}

	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for InGameMenu Widget")) return; }
	MenuInterface->RestartGame();
}

void UInGameMenuWidget::WantsToReturn()
{
	Button_MainMenu->PlayPressedSound();

	if (!WidgetSwitcher || !ReturnMenuPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to ReturnMenuPanel within InGameMenu Widget")) return; }
	Name_LastButton = *Button_MainMenu->GetName();
	bIsAdditionalPanelOpen = true;
	WidgetSwitcher->SetActiveWidget(ReturnMenuPanel);

	if (!Button_CancelReturn) { UE_LOG(LogTemp, Warning, TEXT("Button_CancelReturn is missing from InGameMenu Widget")) return; }
	Button_CancelReturn->SetFocusToButton();
}

void UInGameMenuWidget::CancelReturn()
{
	Button_CancelReturn->PlayPressedSound();
	ReturnToInGameMenu();
}

void UInGameMenuWidget::ConfirmReturn()
{
	Button_ConfirmReturn->PlayPressedSound();

	if (WidgetSwitcher && LoadingScreen)
	{
		WidgetSwitcher->SetActiveWidget(LoadingScreen);
	}

	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for InGameMenu Widget")) return; }
	MenuInterface->ReturnToMainMenu();
}

void UInGameMenuWidget::ReturnToInGameMenu()
{
	if (!WidgetSwitcher || !PausePanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to PausePanel within InGameMenu Widget")) return; }
	bIsAdditionalPanelOpen = false;
	WidgetSwitcher->SetActiveWidget(PausePanel);
	SetWidgetToFocus(Name_LastButton);
}


////////////////////////////////////////////////////////////////////////////////
// Button Hover

void UInGameMenuWidget::ButtonResumeOnHover()
{
	Button_Resume->SetFocusToButton();
}

void UInGameMenuWidget::ButtonOptionsOnHover()
{
	Button_Options->SetFocusToButton();
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


////////////////////////////////////////////////////////////////////////////////
// Inputs

void UInGameMenuWidget::EscInput()
{
	if (!bIsAdditionalPanelOpen)
	{
		CloseMenuInput();
	}
	else
	{
		ReturnToInGameMenu();
	}
}

void UInGameMenuWidget::BackInput()
{
	if (!bIsAdditionalPanelOpen) { return; }
	ReturnToInGameMenu();
}

void UInGameMenuWidget::CloseMenuInput()
{
	ResumeGame();
}

