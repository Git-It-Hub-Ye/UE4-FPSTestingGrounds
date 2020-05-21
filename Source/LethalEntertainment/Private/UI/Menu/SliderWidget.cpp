// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "SliderWidget.h"

#define LOCTEXT_NAMESPACE "Value"

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
	SetTextHeading();
}

void USliderWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USliderWidget::SetInitialValue(float Value)
{
	Value_Current = Value / 100.f;

	SetTextValue();
	UpdateProgressBar(Value_Current);

	if (!Slider) { UE_LOG(LogTemp, Warning, TEXT("Slider missing from Slider Widget")) return; }
	Slider->SetValue(Value_Current);
}

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

void USliderWidget::UpdateValue(float Value)
{
	// Clamp Value between 0.01 & 1 (this way between 1 & 100 is displayed to user)
	SetNewValue(FMath::Clamp<float>(Value, 0.01f, 1.f));
	SetTextValue();
	UpdateProgressBar(FMath::Clamp<float>(Value, 0.01f, 1.f));
}

void USliderWidget::UpdateProgressBar(float Value)
{
	if (!ProgressBar) { UE_LOG(LogTemp, Warning, TEXT("ProgressBar missing from Slider Widget")) return; }
	ProgressBar->SetPercent(Value);
}

#undef LOCTEXT_NAMESPACE

