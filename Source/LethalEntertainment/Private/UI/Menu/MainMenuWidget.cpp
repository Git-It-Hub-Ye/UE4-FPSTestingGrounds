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
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Play is missing from MainMenu Widget")) return false; }

	if (Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UMainMenuWidget::WantsToQuit);

		if (Button_ConfirmQuit)
		{
			Button_ConfirmQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_ConfirmQuit is missing from MainMenu Widget")) return false; }

		if (Button_CancelQuit)
		{
			Button_CancelQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::ReturnToMainMenu);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_CancelQuit is missing from MainMenu Widget")) return false; }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Quit is missing from MainMenu Widget")) return false; }

	if (Button_Controls)
	{
		Button_Controls->OnClicked.AddDynamic(this, &UMainMenuWidget::ViewControls);
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
	Super::NativePreConstruct();
	if (!Button_Play) { UE_LOG(LogTemp, Warning, TEXT("Button_Play is missing from MainMenu Widget")) return; }
	Button_Play->SetKeyboardFocus();
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
	if (!WidgetSwitcher || !QuitMenu) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to QuitMenu in MainMenu Widget")) return; }
	Name_LastButton = *Button_Quit->GetName();
	WidgetSwitcher->SetActiveWidget(QuitMenu);

	if (!Button_CancelQuit) { UE_LOG(LogTemp, Warning, TEXT("Button_CancelQuit is missing from MainMenu Widget")) return; }
	Button_CancelQuit->SetKeyboardFocus();
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

