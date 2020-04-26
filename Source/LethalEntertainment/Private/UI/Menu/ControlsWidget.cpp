// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "MenuButtonsWidget.h"
#include "Components/ScrollBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "ControlsWidget.h"

bool UControlsWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (Button_Back && Button_Back->GetButton())
	{
		Button_Back->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::ReturnToPrevious);
		Button_Back->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonBackOnHover);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Return is missing from Controls Widget")) return false; }

	if (Button_MouseKey && Button_MouseKey->GetButton())
	{
		Button_MouseKey->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::ViewMouseKeyInputs);
		Button_MouseKey->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonMouseKeyOnHover);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_MouseKey is missing from Controls Widget")) }

	if (Button_Controller && Button_Controller->GetButton())
	{
		Button_Controller->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::ViewControllerInputs);
		Button_Controller->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonControllerOnHover);

		if (Button_PS && Button_PS->GetButton())
		{
			Button_PS->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::ShowPSControls);
			Button_PS->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonPSOnHover);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_PS is missing from Controls Widget")) }

		if (Button_XB && Button_XB->GetButton())
		{
			Button_XB->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::ShowXBControls);
			Button_XB->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonXBOnHover);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_XB is missing from Controls Widget")) }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Controller is missing from Controls Widget")) }

	return true;
}

void UControlsWidget::NativeOnFocusLost(const FFocusEvent & InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
}

void UControlsWidget::SetWidgetInterface(IUserWidgetInterface * UserWidgetInt)
{
	UserWidgetInterface = UserWidgetInt;
}

void UControlsWidget::SetFocus()
{
	if (!Button_MouseKey) { UE_LOG(LogTemp, Warning, TEXT("Button_MouseKey is missing from Controls Widget")) return; }
	Button_MouseKey->SetFocusToButton();
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UControlsWidget::ViewMouseKeyInputs()
{
	if (!WidgetSwitcher || !MouseKeyPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to MouseKeyPanel in Controls Widget")) return; }
	Name_LastButton = *Button_MouseKey->GetName();
	
	bIsLookAtBindings_MouseKeys = true;
	WidgetSwitcher->SetActiveWidget(MouseKeyPanel);
}

void UControlsWidget::ViewControllerInputs()
{
	if (!WidgetSwitcher || !GamePadPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to GamePadPanel in Controls Widget")) return; }
	Name_LastButton = *Button_Controller->GetName();
	WidgetSwitcher->SetActiveWidget(GamePadPanel);

	if (Button_PS) 
	{ 
		Button_PS->SetFocusToButton();
	}
	else 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("Button_PS is missing from Controls Widget"))

		if (!Button_XB) { UE_LOG(LogTemp, Warning, TEXT("Button_XB is missing from Controls Widget")) return; }
		Button_XB->SetFocusToButton();
	}
}

void UControlsWidget::ShowPSControls()
{
	if (!WidgetSwitcher_Gamepad || !PSPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to PSPanel in Controls Widget")) return; }
	Name_LastGamepadButton = *Button_PS->GetName();
	bIsLookAtBindings_PS = true;
	WidgetSwitcher_Gamepad->SetActiveWidget(PSPanel);
}

void UControlsWidget::ShowXBControls()
{
	if (!WidgetSwitcher_Gamepad || !XBPanel) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to XBPanel in Controls Widget")) return; }
	Name_LastGamepadButton = *Button_XB->GetName();
	bIsLookAtBindings_XB = true;
	WidgetSwitcher_Gamepad->SetActiveWidget(XBPanel);
}

void UControlsWidget::ReturnToPrevious()
{
	if (WidgetSwitcher_Gamepad && ConTypePanel && WidgetSwitcher_Gamepad->GetActiveWidget() != ConTypePanel)
	{
		ResetLookAtBindings();
		WidgetSwitcher_Gamepad->SetActiveWidget(ConTypePanel);
		SetWidgetToFocus(Name_LastGamepadButton);
	}
	else if (WidgetSwitcher && ControlsPanel && WidgetSwitcher->GetActiveWidget() != ControlsPanel)
	{
		ResetLookAtBindings();
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
// Button Hover

void UControlsWidget::ButtonBackOnHover()
{
	Button_Back->SetFocusToButton();
}

void UControlsWidget::ButtonMouseKeyOnHover()
{
	Button_MouseKey->SetFocusToButton();
}

void UControlsWidget::ButtonControllerOnHover()
{
	Button_Controller->SetFocusToButton();
}

void UControlsWidget::ButtonPSOnHover()
{
	Button_PS->SetFocusToButton();
}

void UControlsWidget::ButtonXBOnHover()
{
	Button_XB->SetFocusToButton();
}

void UControlsWidget::ResetLookAtBindings()
{
	bIsLookAtBindings_MouseKeys = false;
	bIsLookAtBindings_PS = false;
	bIsLookAtBindings_XB = false;
}


////////////////////////////////////////////////////////////////////////////////
// Scroll Inputs

void UControlsWidget::ScrollThroughMenu(bool bShouldScrollUp)
{
	if (bIsLookAtBindings_MouseKeys)
	{
		if (!ScrollBox_MouseKey) { UE_LOG(LogTemp, Warning, TEXT("Scroll_MouseKey missing from Controls Widget")) return; }
		SetMaxScrollLength(ScrollBox_MouseKey, SizeY_ScrollBox_MouseKey);
		MoveScrollBox(ScrollBox_MouseKey, bShouldScrollUp);
	}
	else if (bIsLookAtBindings_PS)
	{
		if (!ScrollBox_PS) { UE_LOG(LogTemp, Warning, TEXT("ScrollBox_PS missing from Controls Widget")) return; }
		SetMaxScrollLength(ScrollBox_PS, SizeY_ScrollBox_PS);
		MoveScrollBox(ScrollBox_PS, bShouldScrollUp);
	}
	else if (bIsLookAtBindings_XB)
	{
		if (!ScrollBox_XB) { UE_LOG(LogTemp, Warning, TEXT("ScrollBox_XB missing from Controls Widget")) return; }
		SetMaxScrollLength(ScrollBox_XB, SizeY_ScrollBox_XB);
		MoveScrollBox(ScrollBox_XB, bShouldScrollUp);
	}
}

void UControlsWidget::MoveScrollBox(UScrollBox * ScrollBox, bool bShouldScrollUp)
{
	if (!ScrollBox) { UE_LOG(LogTemp, Warning, TEXT("Unable to move ScrollBox in Controls Widget")) return; }
	float ScrollOffset = bShouldScrollUp ? ScrollBox->GetScrollOffset() - AmountToAdd : ScrollBox->GetScrollOffset() + AmountToAdd;
	ScrollBox->SetScrollOffset(FMath::Clamp(ScrollOffset, 0.f, MaxScrollLength));
}

bool UControlsWidget::GetIsScrollableMenu()
{
	return bIsLookAtBindings_MouseKeys || bIsLookAtBindings_PS || bIsLookAtBindings_XB;
}

void UControlsWidget::SetMaxScrollLength(UScrollBox * ScrollBox, float SizeY_ScrollBox)
{
	// Get Y size of scroll box including all children
	// Minus Y size of actual scroll box visible (set in editor)
	// Gives the end scroll offset of the scroll box
	MaxScrollLength = ScrollBox->GetDesiredSize().Y - SizeY_ScrollBox;
}

FReply UControlsWidget::NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent)
{
	FReply Result = FReply::Unhandled();
	const FKey Key = InKeyEvent.GetKey();

	if (GetIsScrollableMenu())
	{
		if (Key == EKeys::Down)
		{
			AmountToAdd = 15.f;
			ScrollThroughMenu(false);
			Result = FReply::Handled();
		}
		else if (Key == EKeys::Gamepad_DPad_Down || Key == EKeys::Gamepad_LeftStick_Down)
		{
			AmountToAdd = 30.f;
			ScrollThroughMenu(false);
			Result = FReply::Handled();
		}
		else if (Key == EKeys::Up)
		{
			AmountToAdd = 15.f;
			ScrollThroughMenu(true);
			Result = FReply::Handled();
		}
		else if (Key == EKeys::Gamepad_DPad_Up || Key == EKeys::Gamepad_LeftStick_Up)
		{
			AmountToAdd = 30.f;
			ScrollThroughMenu(true);
			Result = FReply::Handled();
		}
	}
	return Result;
}

