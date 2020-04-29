// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
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
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Play is missing from MainMenu Widget")) return false; }

	if (Button_Quit && Button_Quit->GetButton())
	{
		Button_Quit->GetButton()->OnClicked.AddDynamic(this, &UMainMenuWidget::WantsToQuit);
		Button_Quit->GetButton()->OnHovered.AddDynamic(this, &UMainMenuWidget::ButtonOuitOnHover);

		if (Button_ConfirmQuit && Button_ConfirmQuit->GetButton())
		{
			Button_ConfirmQuit->GetButton()->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
			Button_ConfirmQuit->GetButton()->OnHovered.AddDynamic(this, &UMainMenuWidget::ButtonConfirmQuitOnHover);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmQuit is missing from MainMenu Widget")) return false; }

		if (Button_CancelQuit && Button_CancelQuit->GetButton())
		{
			Button_CancelQuit->GetButton()->OnClicked.AddDynamic(this, &UMainMenuWidget::ReturnToMainMenu);
			Button_CancelQuit->GetButton()->OnHovered.AddDynamic(this, &UMainMenuWidget::ButtonCancelQuitOnHover);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelQuit is missing from MainMenu Widget")) return false; }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Quit is missing from MainMenu Widget")) return false; }

	if (Button_Controls && Button_Controls->GetButton())
	{
		Button_Controls->GetButton()->OnClicked.AddDynamic(this, &UMainMenuWidget::ViewControls);
		Button_Controls->GetButton()->OnHovered.AddDynamic(this, &UMainMenuWidget::ButtonControlsOnHover);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Controls is missing from MainMenu Widget")) }

	return true;
}

void UMainMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (!ControlsPanel) { UE_LOG(LogTemp, Warning, TEXT("Control panel widget missing from MainMenu Widget")) return; }

	ControlsPanel->SetWidgetInterface(this);
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!Button_Play) { UE_LOG(LogTemp, Warning, TEXT("Button_Play is missing from MainMenu Widget")) return; }
	Button_Play->SetFocusToButton();
}

void UMainMenuWidget::RequestReturnToParentWidget()
{
	ReturnToMainMenu();
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UMainMenuWidget::PlayGame()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for MainMenu Widget")) return; }
	MenuInterface->PlayGame();
}

void UMainMenuWidget::WantsToQuit()
{
	if (!WidgetSwitcher || !QuitMenu) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to QuitMenu in MainMenu Widget")) return; }
	Name_LastButton = *Button_Quit->GetName();
	WidgetSwitcher->SetActiveWidget(QuitMenu);

	if (!Button_CancelQuit) { UE_LOG(LogTemp, Warning, TEXT("Button_CancelQuit is missing from MainMenu Widget")) return; }
	Button_CancelQuit->SetFocusToButton();
}

void UMainMenuWidget::QuitGame()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for MainMenu Widget")) return; }
	MenuInterface->OuitGame();
}

void UMainMenuWidget::ViewControls()
{
	if (!WidgetSwitcher || !ControlsPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to ControlsPanel in MainMenu Widget")) return; }
	Name_LastButton = *Button_Controls->GetName();
	WidgetSwitcher->SetActiveWidget(ControlsPanel);
	ControlsPanel->SetFocus();
}

void UMainMenuWidget::ReturnToMainMenu()
{
	if (!WidgetSwitcher || !MainMenu) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to MainMenu in MainMenu Widget")) return; }
	WidgetSwitcher->SetActiveWidget(MainMenu);
	SetWidgetToFocus(Name_LastButton);
}


////////////////////////////////////////////////////////////////////////////////
// Button Hover

void UMainMenuWidget::ButtonPlayOnHover()
{
	Button_Play->SetFocusToButton();
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

void UMainMenuWidget::OnNavigatedToButtonPlay()
{
	if (!Button_Play) { UE_LOG(LogTemp, Warning, TEXT("Button_Play is missing from MainMenu Widget")) return; }
	ButtonPlayOnHover();
}

void UMainMenuWidget::OnNavigatedToButtonControls()
{
	if (!Button_Controls) { UE_LOG(LogTemp, Warning, TEXT("Button_Controls is missing from MainMenu Widget")) return; }
	ButtonControlsOnHover();
}

void UMainMenuWidget::OnNavigatedToButtonQuit()
{
	if (!Button_Quit) { UE_LOG(LogTemp, Warning, TEXT("Button_Quit is missing from MainMenu Widget")) return; }
	ButtonOuitOnHover();
}

