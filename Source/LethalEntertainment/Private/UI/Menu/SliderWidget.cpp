// Copyright 2018 - 2021 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "SliderWidget.h"

#define LOCTEXT_NAMESPACE "Value"


////////////////////////////////////////////////////////////////////////////////
// Setup

bool USliderWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (!Slider) { UE_LOG(LogTemp, Warning, TEXT("Slider_Main is missing from Slider Widget")) return false; }
	Slider->OnValueChanged.AddUniqueDynamic(this, &USliderWidget::UpdateValue);

	return true;
}

void USliderWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetBackgroundColour(Colour_Default);
	SetTextHeading();
}

void USliderWidget::SetInitialValue(float Value)
{
	Value_Current = Value / 100.f;

	SetTextValue();
	UpdateProgressBar(Value_Current);

	if (!Slider) { UE_LOG(LogTemp, Warning, TEXT("Slider missing from Slider Widget")) return; }
	Slider->SetValue(Value_Current);
}


////////////////////////////////////////////////////////////////////////////////
// Focus

void USliderWidget::NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	OnWidgetFocused.Broadcast(this);
	SetBackgroundColour(Colour_Focus);
	PlayFocusSound();
}

void USliderWidget::NativeOnRemovedFromFocusPath(const FFocusEvent & InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	SetBackgroundColour(Colour_Default);
}

void USliderWidget::SetBackgroundColour(FLinearColor Colour)
{
	if (!BackgroundPanel) { UE_LOG(LogTemp, Warning, TEXT("BackgroundPanel missing from Slider Widget")) return; }
	BackgroundPanel->SetBrushColor(Colour);
}


////////////////////////////////////////////////////////////////////////////////
// Inputs

void USliderWidget::SetNewValue(float Value)
{
	Value_Current = Value;
}

void USliderWidget::SetTextHeading()
{
	if (!Text_Option->IsValidLowLevel()) { UE_LOG(LogTemp, Warning, TEXT("Text_Option invalid in Slider Widget")) return; }
	Text_Option->SetText(Text_Edit);
}

void USliderWidget::SetTextValue()
{
	if (!Text_Value->IsValidLowLevel()) { UE_LOG(LogTemp, Warning, TEXT("Text_Value invalid in Slider Widget")) return; }
	Text_Value->SetText(FText::AsNumber(FMath::RoundToInt(Value_Current * 100)));
}

void USliderWidget::UpdateProgressBar(float Value)
{
	if (!ProgressBar) { UE_LOG(LogTemp, Warning, TEXT("ProgressBar missing from Slider Widget")) return; }
	ProgressBar->SetPercent(Value);
}

void USliderWidget::UpdateValue(float Value)
{
	if (Value_Current == FMath::Clamp<float>(Value, 0.01f, 1.f)) { return; }

	// Clamp Value between 0.01 & 1 (this way between 1 & 100 is displayed to user)
	Slider->SetValue(FMath::Clamp<float>(Value, 0.01f, 1.f));
	SetNewValue(FMath::Clamp<float>(Value, 0.01f, 1.f));
	SetTextValue();
	UpdateProgressBar(FMath::Clamp<float>(Value, 0.01f, 1.f));
}

void USliderWidget::AdjustSlider(bool IncreaseSliderValue)
{
	if (!Slider) { UE_LOG(LogTemp, Warning, TEXT("Slider missing from Slider Widget")) return; }

	float NewValue = IncreaseSliderValue ? Slider->GetValue() + Slider->StepSize : Slider->GetValue() - Slider->StepSize;
	UpdateValue(NewValue);
}

FReply USliderWidget::NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent)
{
	FReply Result = FReply::Unhandled();
	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Left || Key == EKeys::Gamepad_DPad_Left || Key == EKeys::Gamepad_LeftStick_Left)
	{
		AdjustSlider(false);
		Result = FReply::Handled();
	}
	else if (Key == EKeys::Right || Key == EKeys::Gamepad_DPad_Right || Key == EKeys::Gamepad_LeftStick_Right)
	{
		AdjustSlider(true);
		Result = FReply::Handled();
	}
	return Result;
}


////////////////////////////////////////////////////////////////////////////////
// SFX

void USliderWidget::PlayFocusSound()
{
	if (!OnFocusSound) { UE_LOG(LogTemp, Warning, TEXT("Focus Sound missing from Button Widget")) return; }
	UGameplayStatics::PlaySound2D(this, OnFocusSound);
}

#undef LOCTEXT_NAMESPACE

