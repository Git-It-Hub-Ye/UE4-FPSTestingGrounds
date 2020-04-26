// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MenuButtonsWidget.h"

void UMenuButtonsWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetupButton();
	SetupText();
}


////////////////////////////////////////////////////////////////////////////////
// Setup

void UMenuButtonsWidget::SetupButton()
{
	if (!Button_Main) { UE_LOG(LogTemp, Warning, TEXT("Button_Main missing from Button Widget")) return; }
	Button_Main->SetBackgroundColor(Button_Colour);
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
	SetOnHoverStyle();	
}

void UMenuButtonsWidget::NativeOnRemovedFromFocusPath(const FFocusEvent & InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	SetUnhoverStyle();
}

void UMenuButtonsWidget::SetOnHoverStyle()
{
	UE_LOG(LogTemp, Warning, TEXT("Hover : %s"), *this->GetName())
}

void UMenuButtonsWidget::SetUnhoverStyle()
{
	UE_LOG(LogTemp, Warning, TEXT("Unhover : %s"), *this->GetName())
}


////////////////////////////////////////////////////////////////////////////////
// Data

UButton * UMenuButtonsWidget::GetButton()
{
	return Button_Main;
}

