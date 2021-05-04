// Copyright 2018 - 2021 Stuart McDonald.

#include "LethalEntertainment.h"
#include "OptionsMenuWidget.h"
#include "ControlsWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "MenuButtonsWidget.h"
#include "MainMenuWidget.h"

bool UMainMenuWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }
	if (Button_Play && Button_Play->GetButton())
	{
		Button_Play->GetButton()->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayGame);
		Button_Play->GetButton()->OnHovered.AddDynamic(this, &UMainMenuWidget::ButtonPlayOnHover);
		Button_Play->OnWidgetFocused.AddUniqueDynamic(this, &UMainMenuWidget::SetFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Play is missing from MainMenu Widget")) return false; }

	if (Button_Quit && Button_Quit->GetButton())
	{
		Button_Quit->GetButton()->OnClicked.AddDynamic(this, &UMainMenuWidget::WantsToQuit);
		Button_Quit->GetButton()->OnHovered.AddDynamic(this, &UMainMenuWidget::ButtonOuitOnHover);
		Button_Quit->OnWidgetFocused.AddUniqueDynamic(this, &UMainMenuWidget::SetCurrentFocusedWidgetName);

		if (Button_ConfirmQuit && Button_ConfirmQuit->GetButton())
		{
			Button_ConfirmQuit->GetButton()->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
			Button_ConfirmQuit->GetButton()->OnHovered.AddDynamic(this, &UMainMenuWidget::ButtonConfirmQuitOnHover);
			Button_ConfirmQuit->OnWidgetFocused.AddUniqueDynamic(this, &UMainMenuWidget::SetCurrentFocusedWidgetName);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmQuit is missing from MainMenu Widget")) return false; }

		if (Button_CancelQuit && Button_CancelQuit->GetButton())
		{
			Button_CancelQuit->GetButton()->OnClicked.AddDynamic(this, &UMainMenuWidget::ReturnToMainMenu);
			Button_CancelQuit->GetButton()->OnHovered.AddDynamic(this, &UMainMenuWidget::ButtonCancelQuitOnHover);
			Button_CancelQuit->OnWidgetFocused.AddUniqueDynamic(this, &UMainMenuWidget::SetCurrentFocusedWidgetName);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelQuit is missing from MainMenu Widget")) return false; }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Quit is missing from MainMenu Widget")) return false; }

	if (Button_Options && Button_Options->GetButton())
	{
		Button_Options->GetButton()->OnClicked.AddDynamic(this, &UMainMenuWidget::ViewOptions);
		Button_Options->GetButton()->OnHovered.AddDynamic(this, &UMainMenuWidget::ButtonOptionsOnHover);
		Button_Options->OnWidgetFocused.AddUniqueDynamic(this, &UMainMenuWidget::SetFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Options is missing from MainMenu Widget")) }

	if (Button_Controls && Button_Controls->GetButton())
	{
		Button_Controls->GetButton()->OnClicked.AddDynamic(this, &UMainMenuWidget::ViewControls);
		Button_Controls->GetButton()->OnHovered.AddDynamic(this, &UMainMenuWidget::ButtonControlsOnHover);
		Button_Controls->OnWidgetFocused.AddUniqueDynamic(this, &UMainMenuWidget::SetFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Controls is missing from MainMenu Widget")) }

	return true;
}

void UMainMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (OptionsPanel)
	{
		OptionsPanel->SetMenuInterface(MenuInterface);
		OptionsPanel->SetUserWidgetInterface(this);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("OptionsPanel widget missing from MainMenu Widget")) }

	if (!ControlsPanel) { UE_LOG(LogTemp, Warning, TEXT("Control panel widget missing from MainMenu Widget")) return; }
	ControlsPanel->SetMenuInterface(MenuInterface);
	ControlsPanel->SetUserWidgetInterface(this);
	ControlsPanel->OnControlTypeSet.AddUniqueDynamic(this, &UMainMenuWidget::SetControlType);
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_Play && Button_Play->GetButton())
	{
		Button_Play->SetFocusToButton();
	}
	else{ UE_LOG(LogTemp, Warning, TEXT("Button_Play is missing from MainMenu Widget")) }
}

void UMainMenuWidget::RequestReturnToParentWidget()
{
	ReturnToMainMenu();
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UMainMenuWidget::PlayGame()
{
	Button_Play->PlayPressedSound();

	if (WidgetSwitcher && LoadingScreen)
	{
		WidgetSwitcher->SetActiveWidget(LoadingScreen);
	}

	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for MainMenu Widget")) return; }
	MenuInterface->PlayGame();
}

void UMainMenuWidget::WantsToQuit()
{
	Button_Quit->PlayPressedSound();

	if (!WidgetSwitcher || !QuitMenu) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to QuitMenu in MainMenu Widget")) return; }
	Name_LastButton = *Button_Quit->GetName();
	bIsAdditionalPanelOpen = true;
	WidgetSwitcher->SetActiveWidget(QuitMenu);

	if (!Button_CancelQuit) { UE_LOG(LogTemp, Warning, TEXT("Button_CancelQuit is missing from MainMenu Widget")) return; }
	Button_CancelQuit->SetFocusToButton();
}

void UMainMenuWidget::QuitGame()
{
	Button_ConfirmQuit->PlayPressedSound();

	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for MainMenu Widget")) return; }
	MenuInterface->OuitGame();
}

void UMainMenuWidget::ViewOptions()
{
	Button_Options->PlayPressedSound();

	if (!WidgetSwitcher || !OptionsPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to Switch to OptionsPanel within MainMenu Widget")) return; }
	Name_LastButton = *Button_Options->GetName();
	OptionsPanel->SetInitialValues();
	WidgetSwitcher->SetActiveWidget(OptionsPanel);
	OptionsPanel->SetFocus(Current_ControlType);
}

void UMainMenuWidget::ViewControls()
{
	Button_Controls->PlayPressedSound();

	if (!WidgetSwitcher || !ControlsPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to ControlsPanel in MainMenu Widget")) return; }
	Name_LastButton = *Button_Controls->GetName();
	WidgetSwitcher->SetActiveWidget(ControlsPanel);
	ControlsPanel->SetFocus();
	ControlsPanel->SetScrollBoxType(Current_ControlType);
}

void UMainMenuWidget::ReturnToMainMenu()
{
	Button_CancelQuit->PlayPressedSound();

	if (!WidgetSwitcher || !MainMenu) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to MainMenu in MainMenu Widget")) return; }
	bIsAdditionalPanelOpen = false;
	WidgetSwitcher->SetActiveWidget(MainMenu);
	SetWidgetToFocus(Name_LastButton);
}


////////////////////////////////////////////////////////////////////////////////
// Button Hover

void UMainMenuWidget::ButtonPlayOnHover()
{
	Button_Play->SetFocusToButton();
}

void UMainMenuWidget::ButtonOptionsOnHover()
{
	Button_Options->SetFocusToButton();
}

void UMainMenuWidget::ButtonControlsOnHover()
{
	Button_Controls->SetFocusToButton();
}

void UMainMenuWidget::ButtonOuitOnHover()
{
	Button_Quit->SetFocusToButton();
}

void UMainMenuWidget::ButtonConfirmQuitOnHover()
{
	Button_ConfirmQuit->SetFocusToButton();
}

void UMainMenuWidget::ButtonCancelQuitOnHover()
{
	Button_CancelQuit->SetFocusToButton();
}


////////////////////////////////////////////////////////////////////////////////
// Navigation Functions

void UMainMenuWidget::OnNavigateToButton()
{
	UMenuButtonsWidget * Button = Cast<UMenuButtonsWidget>(GetWidgetFromName(Name_NavToButton));
	if (!Button) { UE_LOG(LogTemp, Warning, TEXT("Can't Navigate back to buttons from Quit")) return; }
	Button->SetFocusToButton();
}

void UMainMenuWidget::OnNavigatedToButtonQuit()
{
	if (!Button_Quit) { UE_LOG(LogTemp, Warning, TEXT("Button_Quit is missing from MainMenu Widget")) return; }
	Name_NavToButton = Name_CurrentFocusedWidget;
	ButtonOuitOnHover();
}


////////////////////////////////////////////////////////////////////////////////
// Focus

void UMainMenuWidget::SetFocusedWidgetName(UWidget * Widget)
{
	SetCurrentFocusedWidgetName(Widget);
	if (!Widget) { return; }
	Name_NavToButton = *Widget->GetName();
}


////////////////////////////////////////////////////////////////////////////////
// Inputs

void UMainMenuWidget::EscInput()
{
	if (!bIsAdditionalPanelOpen)
	{
		WantsToQuit();
	}
	else
	{
		ReturnToMainMenu();
	}
}

