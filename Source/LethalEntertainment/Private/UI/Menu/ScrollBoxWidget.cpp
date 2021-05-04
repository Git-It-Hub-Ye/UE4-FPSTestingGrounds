// Copyright 2018 - 2021 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/ScrollBox.h"
#include "ScrollBoxWidget.h"

void UScrollBoxWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetupScrollBar();
}


////////////////////////////////////////////////////////////////////////////////
// Setup

void UScrollBoxWidget::SetupScrollBar()
{
	if (!ScrollBox_Main) { UE_LOG(LogTemp, Warning, TEXT("Scroll Box missing from UScrollBox Widget")) return; }
	ScrollBox_Main->WidgetBarStyle.SetNormalThumbImage(Colour_Default);
	ScrollBox_Main->WidgetBarStyle.SetHoveredThumbImage(Colour_Focus);
}

void UScrollBoxWidget::SetUserWidgetInterface(IUserWidgetInterface * WidgetInterface)
{
	UserWidgetInterface = WidgetInterface;
}

void UScrollBoxWidget::SetSizeYOfScrollBox(float SizeY)
{
	ScrollBoxSizeY = SizeY;
}

void UScrollBoxWidget::ResetOffset()
{
	if (!ScrollBox_Main) { UE_LOG(LogTemp, Warning, TEXT("Scroll Box missing from UScrollBox Widget")) return; }
	ScrollBox_Main->SetScrollOffset(0.f);
}


////////////////////////////////////////////////////////////////////////////////
// Focus

void UScrollBoxWidget::NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	OnWidgetFocused.Broadcast(this);
	SetFocusStyle();
}

void UScrollBoxWidget::NativeOnRemovedFromFocusPath(const FFocusEvent & InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	SetUnfocusStyle();
}

void UScrollBoxWidget::SetFocusStyle()
{
	if (!ScrollBox_Main) { UE_LOG(LogTemp, Warning, TEXT("Scroll Box missing from UScrollBox Widget")) return; }
	ScrollBox_Main->WidgetBarStyle.SetNormalThumbImage(Colour_Focus);
}

void UScrollBoxWidget::SetUnfocusStyle()
{
	if (!ScrollBox_Main) { UE_LOG(LogTemp, Warning, TEXT("Scroll Box missing from UScrollBox Widget")) return; }
	ScrollBox_Main->WidgetBarStyle.SetNormalThumbImage(Colour_Default);
}

void UScrollBoxWidget::OnNavUp()
{
	if (!UserWidgetInterface) { UE_LOG(LogTemp, Warning, TEXT("UserWidgetInterface missing from ScrollBox widget")) return; }
	UserWidgetInterface->OnNavUpToParent();
}

void UScrollBoxWidget::OnNavDown()
{
	if (!UserWidgetInterface) { UE_LOG(LogTemp, Warning, TEXT("UserWidgetInterface missing from ScrollBox widget")) return; }
	UserWidgetInterface->OnNavDownToParent();
}


////////////////////////////////////////////////////////////////////////////////
// Scroll Inputs

void UScrollBoxWidget::ScrollThroughMenu()
{
	if (!ScrollBox_Main) { UE_LOG(LogTemp, Warning, TEXT("Unable to move ScrollBox in Controls Widget")) return; }

	GetMaxScrollOffset(ScrollBoxSizeY);

	if (ScrollBox_Main->GetScrollOffset() == 0.f && AmountToAdd < 0.f) { OnNavUp(); }
	else if (ScrollBox_Main->GetScrollOffset() == MaxScrollOffset && AmountToAdd > 0.f) { OnNavDown(); }

	float ScrollOffset = ScrollBox_Main->GetScrollOffset() + AmountToAdd;
	ScrollBox_Main->SetScrollOffset(FMath::Clamp<float>(ScrollOffset, 0.f, MaxScrollOffset));
}

void UScrollBoxWidget::GetMaxScrollOffset(float SizeY)
{
	// Get Y size of scroll box including all children
	// Minus Y size of actual scroll box visible (set in editor)
	// Gives the end scroll offset of the scroll box
	MaxScrollOffset = ScrollBox_Main->GetDesiredSize().Y - SizeY;
}

void UScrollBoxWidget::UserScrolled()
{
	SetKeyboardFocus();
}

FReply UScrollBoxWidget::NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent)
{
	FReply Result = FReply::Unhandled();
	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Down)
	{
		AmountToAdd = 15.f;
		ScrollThroughMenu();
		Result = FReply::Handled();
	}
	else if (Key == EKeys::Gamepad_DPad_Down || Key == EKeys::Gamepad_LeftStick_Down)
	{
		AmountToAdd = 30.f;
		ScrollThroughMenu();
		Result = FReply::Handled();
	}
	else if (Key == EKeys::Up)
	{
		AmountToAdd = -15.f;
		ScrollThroughMenu();
		Result = FReply::Handled();
	}
	else if (Key == EKeys::Gamepad_DPad_Up || Key == EKeys::Gamepad_LeftStick_Up)
	{
		AmountToAdd = -30.f;
		ScrollThroughMenu();
		Result = FReply::Handled();
	}

	return Result;
}

