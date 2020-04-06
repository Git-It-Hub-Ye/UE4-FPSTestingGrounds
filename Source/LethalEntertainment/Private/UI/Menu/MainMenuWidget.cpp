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
	else { return false; }

	if (Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UMainMenuWidget::WantsToQuit);

		if (Button_ConfirmQuit)
		{
			Button_ConfirmQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
		}
		if (Button_CancelQuit)
		{
			Button_CancelQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::ReturnToMainMenu);
		}
	}
	if (Button_Controls)
	{
		Button_Controls->OnClicked.AddDynamic(this, &UMainMenuWidget::ViewControls);
	}

	return true;
}

void UMainMenuWidget::NativePreConstruct()
{
	if (!ControlsPanel) { UE_LOG(LogTemp, Warning, TEXT("Control panel widget missing")) return; }

	ControlsPanel->SetWidgetInterface(this);
}

void UMainMenuWidget::RequestReturnToParentWidget()
{
	ReturnToMainMenu();
}

void UMainMenuWidget::PlayGame()
{
	if (!MenuInterface) { return; }
	MenuInterface->PlayGame();
}

void UMainMenuWidget::WantsToQuit()
{
	if (!WidgetSwitcher || !QuitMenu) { return; }
	WidgetSwitcher->SetActiveWidget(QuitMenu);
}

void UMainMenuWidget::QuitGame()
{
	if (!MenuInterface) { return; }
	MenuInterface->OuitGame();
}

void UMainMenuWidget::ViewControls()
{
	if (!WidgetSwitcher || !ControlsPanel) { return; }
	WidgetSwitcher->SetActiveWidget(ControlsPanel);
	ShowOrHideQuitButton(false);
}

void UMainMenuWidget::ReturnToMainMenu()
{
	if (!WidgetSwitcher || !MainMenu) { return; }
	WidgetSwitcher->SetActiveWidget(MainMenu);
	ShowOrHideQuitButton(true);
}

void UMainMenuWidget::ShowOrHideQuitButton(bool bShowButton)
{
	if (!Button_Quit) { return; }

	if (bShowButton)
	{
		Button_Quit->SetVisibility(ESlateVisibility::Visible);
	}
	else if (!bShowButton)
	{
		Button_Quit->SetVisibility(ESlateVisibility::Hidden);
	}
}

