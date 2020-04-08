// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "ControlsWidget.h"

void UControlsWidget::NativePreConstruct()
{
	if (Button_Return)
	{
		Button_Return->OnClicked.AddDynamic(this, &UControlsWidget::ReturnToPrevious);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Return is missing from Controls Widget")) }

	if (Button_MouseKey)
	{
		Button_MouseKey->OnClicked.AddDynamic(this, &UControlsWidget::ViewMouseKeyInputs);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_MouseKey is missing from Controls Widget")) }

	if (Button_Controller)
	{
		Button_Controller->OnClicked.AddDynamic(this, &UControlsWidget::ViewControllerInputs);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Controller is missing from Controls Widget")) }

	if (Button_PS)
	{
		Button_PS->OnClicked.AddDynamic(this, &UControlsWidget::ShowPSControls);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_PS is missing from Controls Widget")) }

	if (Button_XB)
	{
		Button_XB->OnClicked.AddDynamic(this, &UControlsWidget::ShowXBControls);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_XB is missing from Controls Widget")) }
}

void UControlsWidget::SetWidgetInterface(IUserWidgetInterface * UserWidgetInt)
{
	UserWidgetInterface = UserWidgetInt;
}

void UControlsWidget::ViewMouseKeyInputs()
{
	if (!WidgetSwitcher || !MouseKeyPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to MouseKeyPanel in Controls Widget")) return; }
	WidgetSwitcher->SetActiveWidget(MouseKeyPanel);
}

void UControlsWidget::ViewControllerInputs()
{
	if (!WidgetSwitcher || !GamePadPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to GamePadPanel in Controls Widget")) return; }
	WidgetSwitcher->SetActiveWidget(GamePadPanel);
}

void UControlsWidget::ReturnToPrevious()
{
	if (WidgetSwitcher_Gamepad && ConTypePanel && WidgetSwitcher_Gamepad->GetActiveWidget() != ConTypePanel)
	{
		WidgetSwitcher_Gamepad->SetActiveWidget(ConTypePanel);
	}
	else if (WidgetSwitcher && ControlsPanel && WidgetSwitcher->GetActiveWidget() != ControlsPanel)
	{
		WidgetSwitcher->SetActiveWidget(ControlsPanel);
	}
	else if (UserWidgetInterface)
	{
		UserWidgetInterface->RequestReturnToParentWidget();
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Unable to Return in Controls Widget")) }
}

void UControlsWidget::ShowPSControls()
{
	if (!WidgetSwitcher_Gamepad || !PSPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to PSPanel in Controls Widget")) return; }
	WidgetSwitcher_Gamepad->SetActiveWidget(PSPanel);
}

void UControlsWidget::ShowXBControls()
{
	if (!WidgetSwitcher_Gamepad || !XBPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to XBPanel in Controls Widget")) return; }
	WidgetSwitcher_Gamepad->SetActiveWidget(XBPanel);
}

