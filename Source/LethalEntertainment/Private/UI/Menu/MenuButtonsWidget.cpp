// Copyright 2018 - 2021 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MenuButtonsWidget.h"


////////////////////////////////////////////////////////////////////////////////
// Setup

void UMenuButtonsWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetupButton();
	SetupText();
}

void UMenuButtonsWidget::SetupButton()
{
	if (!Button_Main) { UE_LOG(LogTemp, Warning, TEXT("Button_Main missing from Button Widget")) return; }
	
	// Set Normal & Hovered same colour. (Prevents mouse hovering from changing colour when player is using keyboard/gamepad)
	Style_Default.SetNormal(Colour_Default);
	Style_Default.SetHovered(Colour_Default);

	// Set Normal & Hovered same colour. (Normal for Keyboard/Gamepad & Hovered for mouse interaction)
	Style_Focused.SetNormal(Colour_Hover);
	Style_Focused.SetHovered(Colour_Hover);
	Style_Focused.SetPressed(Colour_Pressed);

	Button_Main->SetStyle(Style_Default);
}

void UMenuButtonsWidget::SetupText()
{
	if (!Button_Text->IsValidLowLevel()) { UE_LOG(LogTemp, Warning, TEXT("Button_Text invalid in Button Widget")) return; }

	Button_Text->SetText(Text_Edit);
	Button_Text->SetColorAndOpacity(Text_Colour);
	Button_Text->SetRenderTransform(Text_Transform);
	Button_Text->SetFont(Text_Font);
}


////////////////////////////////////////////////////////////////////////////////
// Focus

void UMenuButtonsWidget::SetFocusToButton()
{
	if (!Button_Main) { UE_LOG(LogTemp, Warning, TEXT("Button_Main missing from Button Widget")) return; }
	Button_Main->SetKeyboardFocus();
}

void UMenuButtonsWidget::NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	OnWidgetFocused.Broadcast(this);
	PlayHoverSound();
	SetOnHoverStyle();	
}

void UMenuButtonsWidget::NativeOnRemovedFromFocusPath(const FFocusEvent & InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	SetUnhoverStyle();
}

void UMenuButtonsWidget::SetOnHoverStyle()
{
	Button_Main->SetStyle(Style_Focused);
}

void UMenuButtonsWidget::SetUnhoverStyle()
{
	Button_Main->SetStyle(Style_Default);
}

void UMenuButtonsWidget::PlayPressedSound()
{
	if (!OnPressedSound) { UE_LOG(LogTemp, Warning, TEXT("Pressed Sound missing from Button Widget")) return; }
	UGameplayStatics::PlaySound2D(this, OnPressedSound);
}

void UMenuButtonsWidget::PlayHoverSound()
{
	if (!OnHoverSound) { UE_LOG(LogTemp, Warning, TEXT("Hover Sound missing from Button Widget")) return; }
	UGameplayStatics::PlaySound2D(this, OnHoverSound);
}

float UMenuButtonsWidget::GetPressedSoundDuration()
{
	float Duration = 0.2f;

	if (OnPressedSound) 
	{
		Duration = OnPressedSound->Duration;
	}

	return Duration;
}


////////////////////////////////////////////////////////////////////////////////
// Data

UButton * UMenuButtonsWidget::GetButton()
{
	return Button_Main;
}

