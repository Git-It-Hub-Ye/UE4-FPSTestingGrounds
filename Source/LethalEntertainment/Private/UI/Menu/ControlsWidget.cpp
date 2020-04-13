// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "ControlsWidget.h"

bool UControlsWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (Button_Return)
	{
		Button_Return->OnClicked.AddDynamic(this, &UControlsWidget::ReturnToPrevious);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Return is missing from Controls Widget")) return false; }

	if (Button_MouseKey)
	{
		Button_MouseKey->OnClicked.AddDynamic(this, &UControlsWidget::ViewMouseKeyInputs);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_MouseKey is missing from Controls Widget")) }

	if (Button_Controller)
	{
		Button_Controller->OnClicked.AddDynamic(this, &UControlsWidget::ViewControllerInputs);

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
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Controller is missing from Controls Widget")) }

	return true;
}

void UControlsWidget::SetWidgetInterface(IUserWidgetInterface * UserWidgetInt)
{
	UserWidgetInterface = UserWidgetInt;
}

void UControlsWidget::SetFocus()
{
	if (!Button_MouseKey) { UE_LOG(LogTemp, Warning, TEXT("Button_MouseKey is missing from Controls Widget")) return; }
	Button_MouseKey->SetKeyboardFocus();
}

void UControlsWidget::ViewMouseKeyInputs()
{
	if (!WidgetSwitcher || !MouseKeyPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to MouseKeyPanel in Controls Widget")) return; }
	Name_LastButton = *Button_MouseKey->GetName();
	WidgetSwitcher->SetActiveWidget(MouseKeyPanel);
}

void UControlsWidget::ViewControllerInputs()
{
	if (!WidgetSwitcher || !GamePadPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to GamePadPanel in Controls Widget")) return; }
	Name_LastButton = *Button_Controller->GetName();
	WidgetSwitcher->SetActiveWidget(GamePadPanel);

	if (Button_PS) 
	{ 
		Button_PS->SetKeyboardFocus(); 
	}
	else 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("Button_PS is missing from Controls Widget"))

		if (!Button_XB) { UE_LOG(LogTemp, Warning, TEXT("Button_XB is missing from Controls Widget")) return; }
		Button_XB->SetKeyboardFocus();
	}
}

void UControlsWidget::ShowPSControls()
{
	if (!WidgetSwitcher_Gamepad || !PSPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to PSPanel in Controls Widget")) return; }
	Name_LastGamepadButton = *Button_PS->GetName();
	WidgetSwitcher_Gamepad->SetActiveWidget(PSPanel);
}

void UControlsWidget::ShowXBControls()
{
	if (!WidgetSwitcher_Gamepad || !XBPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to XBPanel in Controls Widget")) return; }
	Name_LastGamepadButton = *Button_XB->GetName();
	WidgetSwitcher_Gamepad->SetActiveWidget(XBPanel);
}

void UControlsWidget::ReturnToPrevious()
{
	if (WidgetSwitcher_Gamepad && ConTypePanel && WidgetSwitcher_Gamepad->GetActiveWidget() != ConTypePanel)
	{
		WidgetSwitcher_Gamepad->SetActiveWidget(ConTypePanel);
		SetWidgetToFocus(Name_LastGamepadButton);
	}
	else if (WidgetSwitcher && ControlsPanel && WidgetSwitcher->GetActiveWidget() != ControlsPanel)
	{
		WidgetSwitcher->SetActiveWidget(ControlsPanel);
		SetWidgetToFocus(Name_LastButton);
	}
	else if (UserWidgetInterface)
	{
		UserWidgetInterface->RequestReturnToParentWidget();
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Unable to Return in Controls Widget")) }
}


////////////////////////////////////////////////////////////////////////////////
// Inputs

void UControlsWidget::ScrollBoxDown()
{
	if (!Scroll_MouseKey) { UE_LOG(LogTemp, Warning, TEXT("Scroll_MouseKey missing from Controls Widget")) return; }
	UE_LOG(LogTemp, Warning, TEXT("Down"))
}

FReply UControlsWidget::NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent)
{
	FReply Result = FReply::Unhandled();

	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Down)
	{
		ScrollBoxDown();
		Result = FReply::Handled();
	}
	
		return Result;
}

