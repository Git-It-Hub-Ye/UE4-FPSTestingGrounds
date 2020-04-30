// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "MenuButtonsWidget.h"
#include "Components/TextBlock.h"
#include "IconsWidget.h"
#include "Components/ScrollBox.h"
#include "ControlsWidget.h"

#define LOCTEXT_NAMESPACE "PlayerHud"

bool UControlsWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (Button_Back && Button_Back->GetButton())
	{
		Button_Back->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::ReturnToPrevious);
		Button_Back->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonBackOnHover);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Return is missing from Controls Widget")) return false; }

	if (Button_ControlType && Button_ControlType->GetButton())
	{
		Button_ControlType->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::SelectControlType);
		Button_ControlType->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonControlTypeOnHover);

		if (Button_MouseKey && Button_MouseKey->GetButton())
		{
			Button_MouseKey->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::ShowMouseKeyInputs);
			Button_MouseKey->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonMouseKeyOnHover);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_MouseKey is missing from Controls Widget")) }

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
	else { UE_LOG(LogTemp, Warning, TEXT("Button_ControlType is missing from Controls Widget")) }

	return true;
}

void UControlsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!ConTypePanel) { UE_LOG(LogTemp, Warning, TEXT("ConTypePanel is missing from GameOverMenu Widget")) return; }
	ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
	SetControlTypeButton();
}

void UControlsWidget::SetControlTypeButton()
{
	if (Button_MouseKey) { ShowMouseKeyInputs(); }
	else if (Button_PS) { ShowPSControls(); }
	else if (Button_XB) { ShowXBControls(); }
	else { UE_LOG(LogTemp, Warning, TEXT("Controller types selection is missing from Controls Widget")) }
}

void UControlsWidget::SetWidgetInterface(IUserWidgetInterface * UserWidgetInt)
{
	UserWidgetInterface = UserWidgetInt;
}

void UControlsWidget::SetFocus()
{
	if (!Button_ControlType) { UE_LOG(LogTemp, Warning, TEXT("Button_MouseKey is missing from Controls Widget")) return; }
	Button_ControlType->SetFocusToButton();
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UControlsWidget::SelectControlType()
{
	if (!ConTypePanel) { UE_LOG(LogTemp, Warning, TEXT("ConTypePanel is missing from Controls Widget")) return; }
	if (bDisableScrollInput)
	{
		ReturnToPrevious();
	}
	else
	{
		bDisableScrollInput = true;
		ConTypePanel->SetVisibility(ESlateVisibility::Visible);
		SetWidgetToFocus(Name_CurrentControlType);
	}
	
}

void UControlsWidget::ShowMouseKeyInputs()
{
	Name_CurrentControlType = *Button_MouseKey->GetName();
	ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
	SetFocus();

	if (!WidgetSwitcher || !ScrollBox_MouseKey) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to ScrollBox_MouseKey in Controls Widget")) return; }
	ResetLookAtBindings();
	bIsLookAtBindings_MouseKeys = true;
	WidgetSwitcher->SetActiveWidget(ScrollBox_MouseKey);
	ScrollBox_MouseKey->SetScrollOffset(0.f);

	Button_ControlType->GetTextToCustomise()->SetText(LOCTEXT("Control Type", "Mouse & Keyboard"));
	SetControlsIcon(Icon_Key);
}

void UControlsWidget::ShowPSControls()
{
	Name_CurrentControlType = *Button_PS->GetName();
	ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
	SetFocus();

	if (!WidgetSwitcher || !ScrollBox_PS) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to ScrollBox_PS in Controls Widget")) return; }
	ResetLookAtBindings();
	bIsLookAtBindings_PS = true;
	WidgetSwitcher->SetActiveWidget(ScrollBox_PS);
	ScrollBox_PS->SetScrollOffset(0.f);

	Button_ControlType->GetTextToCustomise()->SetText(LOCTEXT("Control Type", "Playstation Controller"));
	SetControlsIcon(Icon_PS);
}

void UControlsWidget::ShowXBControls()
{
	Name_CurrentControlType = *Button_XB->GetName();
	ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
	SetFocus();

	if (!WidgetSwitcher || !ScrollBox_XB) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch to ScrollBox_XB in Controls Widget")) return; }
	ResetLookAtBindings();
	bIsLookAtBindings_XB = true;
	WidgetSwitcher->SetActiveWidget(ScrollBox_XB);
	ScrollBox_XB->SetScrollOffset(0.f);

	Button_ControlType->GetTextToCustomise()->SetText(LOCTEXT("Control Type", "Xbox Controller"));
	SetControlsIcon(Icon_XB);
}

void UControlsWidget::ReturnToPrevious()
{
	if (ConTypePanel && ConTypePanel->GetVisibility() == ESlateVisibility::Visible)
	{
		ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
		bDisableScrollInput = false;
		SetFocus();
	}
	else if (UserWidgetInterface)
	{
		UserWidgetInterface->RequestReturnToParentWidget();
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Unable to Return from Controls Widget")) }
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

void UControlsWidget::ButtonControlTypeOnHover()
{
	Button_ControlType->SetFocusToButton();
}

void UControlsWidget::ButtonPSOnHover()
{
	Button_PS->SetFocusToButton();
}

void UControlsWidget::ButtonXBOnHover()
{
	Button_XB->SetFocusToButton();
}


////////////////////////////////////////////////////////////////////////////////
// Scroll Inputs

void UControlsWidget::ResetLookAtBindings()
{
	bDisableScrollInput = false;
	bIsLookAtBindings_MouseKeys = false;
	bIsLookAtBindings_PS = false;
	bIsLookAtBindings_XB = false;
}

void UControlsWidget::SetControlsIcon(UIconsWidget * Icon)
{
	if (!WidgetSwitcherIcons || !Icon) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch Control Icons in Controls Widget")) }
	WidgetSwitcherIcons->SetActiveWidget(Icon);
}


////////////////////////////////////////////////////////////////////////////////
// Scroll Inputs

void UControlsWidget::ScrollThroughMenu(bool bShouldScrollUp)
{
	if (bIsLookAtBindings_MouseKeys)
	{
		if (!ScrollBox_MouseKey) { UE_LOG(LogTemp, Warning, TEXT("Scroll_MouseKey missing from Controls Widget")) return; }
		SetMaxScrollLength(ScrollBox_MouseKey, ScrollBoxData.SizeY_ScrollBox_MouseKey);
		MoveScrollBox(ScrollBox_MouseKey, bShouldScrollUp);
	}
	else if (bIsLookAtBindings_PS)
	{
		if (!ScrollBox_PS) { UE_LOG(LogTemp, Warning, TEXT("ScrollBox_PS missing from Controls Widget")) return; }
		SetMaxScrollLength(ScrollBox_PS, ScrollBoxData.SizeY_ScrollBox_PS);
		MoveScrollBox(ScrollBox_PS, bShouldScrollUp);
	}
	else if (bIsLookAtBindings_XB)
	{
		if (!ScrollBox_XB) { UE_LOG(LogTemp, Warning, TEXT("ScrollBox_XB missing from Controls Widget")) return; }
		SetMaxScrollLength(ScrollBox_XB, ScrollBoxData.SizeY_ScrollBox_XB);
		MoveScrollBox(ScrollBox_XB, bShouldScrollUp);
	}
}

void UControlsWidget::MoveScrollBox(UScrollBox * ScrollBox, bool bShouldScrollUp)
{
	if (!ScrollBox) { UE_LOG(LogTemp, Warning, TEXT("Unable to move ScrollBox in Controls Widget")) return; }

	if (ScrollBox->GetScrollOffset() == 0.f && bShouldScrollUp) { OnNavToConTypeButton(); }
	else if (ScrollBox->GetScrollOffset() == MaxScrollLength && !bShouldScrollUp) { OnNavToBackButton(); }

	float ScrollOffset = bShouldScrollUp ? ScrollBox->GetScrollOffset() - AmountToAdd : ScrollBox->GetScrollOffset() + AmountToAdd;
	ScrollBox->SetScrollOffset(FMath::Clamp<float>(ScrollOffset, 0.f, MaxScrollLength));
}

void UControlsWidget::SetMaxScrollLength(UScrollBox * ScrollBox, float SizeY_ScrollBox)
{
	// Get Y size of scroll box including all children
	// Minus Y size of actual scroll box visible (set in editor)
	// Gives the end scroll offset of the scroll box
	MaxScrollLength = ScrollBox->GetDesiredSize().Y - SizeY_ScrollBox;
}

bool UControlsWidget::GetIsScrollableMenu()
{
	return bIsLookAtBindings_MouseKeys || bIsLookAtBindings_PS || bIsLookAtBindings_XB;
}

FReply UControlsWidget::NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent)
{
	FReply Result = FReply::Unhandled();
	const FKey Key = InKeyEvent.GetKey();

	if (GetIsScrollableMenu() && !bDisableScrollInput)
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


////////////////////////////////////////////////////////////////////////////////
// Navigation Functions

void UControlsWidget::OnNavigatedToScrollMenu()
{
	if (!GetLocalPlayerController()) { UE_LOG(LogTemp, Warning, TEXT("Local PC is missing from Menu Widget")) return; }
	SetUserFocus(GetLocalPlayerController());
}

void UControlsWidget::OnNavToConTypeButton()
{
	if (!Button_ControlType) { UE_LOG(LogTemp, Warning, TEXT("Button_ControlType is missing from Controls Widget")) return; }
	ButtonControlTypeOnHover();
}

void UControlsWidget::OnNavToBackButton()
{
	if (!Button_Back) { UE_LOG(LogTemp, Warning, TEXT("Button_Back is missing from Controls Widget")) return; }
	ButtonBackOnHover();
}

#undef LOCTEXT_NAMESPACE

