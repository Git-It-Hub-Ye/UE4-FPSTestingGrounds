// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
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
			Button_CancelQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::CancelQuit);
		}
	}
	if (Button_Controls)
	{
		Button_Controls->OnClicked.AddDynamic(this, &UMainMenuWidget::ViewControls);

		if (Button_Return)
		{
			Button_Return->OnClicked.AddDynamic(this, &UMainMenuWidget::ReturnToPrevious);
		}
	}

	return true;
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

void UMainMenuWidget::CancelQuit()
{
	if (!WidgetSwitcher || !MainMenu) { return; }
	WidgetSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenuWidget::ViewControls()
{
	if (!WidgetSwitcher || !ControlsPanel) { return; }
	WidgetSwitcher->SetActiveWidget(ControlsPanel);
}

void UMainMenuWidget::ReturnToPrevious()
{
	if (!WidgetSwitcher || !MainMenu) { return; }
	WidgetSwitcher->SetActiveWidget(MainMenu);
}

