// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "MenuButtonsWidget.h"
#include "SliderWidget.h"
#include "OptionsMenuWidget.h"


bool UOptionsMenuWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	this->SetVisibility(ESlateVisibility::Visible);

	if (Button_Apply && Button_Apply->GetButton())
	{
		Button_Apply->GetButton()->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ApplyChanges);
		Button_Apply->GetButton()->OnHovered.AddDynamic(this, &UOptionsMenuWidget::ButtonApplyOnHover);
		Button_Apply->OnWidgetFocused.AddUniqueDynamic(this, &UOptionsMenuWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Apply is missing from OptionsMenu Widget")) return false; }

	if (Button_Reset && Button_Reset->GetButton())
	{
		Button_Reset->GetButton()->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ResetToDefaults);
		Button_Reset->GetButton()->OnHovered.AddDynamic(this, &UOptionsMenuWidget::ButtonResetOnHover);
		Button_Reset->OnWidgetFocused.AddUniqueDynamic(this, &UOptionsMenuWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Reset is missing from OptionsMenu Widget")) return false; }

	if (Button_Back && Button_Back->GetButton())
	{
		Button_Back->GetButton()->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ReturnToPrevious);
		Button_Back->GetButton()->OnHovered.AddDynamic(this, &UOptionsMenuWidget::ButtonBackOnHover);
		Button_Back->OnWidgetFocused.AddUniqueDynamic(this, &UOptionsMenuWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Back is missing from OptionsMenu Widget")) return false; }
	
	return true;
}

void UOptionsMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (Slider_MouseSens)
	{
		Slider_MouseSens->OnWidgetFocused.AddUniqueDynamic(this, &UOptionsMenuWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Slider_MouseSens is missing from OptionsMenu Widget")) }

	if (!Slider_ConSens) { UE_LOG(LogTemp, Warning, TEXT("Slider_ConSens is missing from OptionsMenu Widget")) return; }
	Slider_ConSens->OnWidgetFocused.AddUniqueDynamic(this, &UOptionsMenuWidget::SetCurrentFocusedWidgetName);
}

void UOptionsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOptionsMenuWidget::SetUserWidgetInterface(IUserWidgetInterface * UserWidgetInt)
{
	UserWidgetInterface = UserWidgetInt;
}

void UOptionsMenuWidget::SetInitialValues()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("MenuInterface is missing from OptionsMenu Widget")) return; }
	MenuInterface->GetCurrentUserValues(Current_MouseSens, Current_ConSens);
	MenuInterface->GetDefaultUserValues(Default_MouseSens, Default_ConSens);
	SetUserSettingsValue(Current_MouseSens, Current_ConSens);
}

void UOptionsMenuWidget::SetFocus()
{
	if (Slider_MouseSens)
	{
		Slider_MouseSens->SetKeyboardFocus();
	}
}

void UOptionsMenuWidget::SetUserSettingsValue(float MouseSens, float ConSens)
{
	if (Slider_MouseSens)
	{
		Slider_MouseSens->SetInitialValue(MouseSens);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("No Slider_MouseSens is missing from OptionsMenu Widget")) }

	if (Slider_ConSens)
	{
		Slider_ConSens->SetInitialValue(ConSens);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("No Slider_ConSens is missing from OptionsMenu Widget")) }
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UOptionsMenuWidget::ApplyChanges()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("MenuInterface is missing from OptionsMenu Widget")) return; }
	if (Slider_MouseSens) { Current_MouseSens = Slider_MouseSens->GetCurrentValue(); }
	if (Slider_ConSens) { Current_ConSens = Slider_ConSens->GetCurrentValue(); }

	MenuInterface->SetNewUserSettings(Current_MouseSens, Current_ConSens);
}

void UOptionsMenuWidget::ResetToDefaults()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("MenuInterface is missing from OptionsMenu Widget")) return; }
	MenuInterface->SetNewUserSettings(Default_MouseSens, Default_ConSens);
	SetUserSettingsValue(Default_MouseSens, Default_ConSens);
}

void UOptionsMenuWidget::ReturnToPrevious()
{
	if (UserWidgetInterface)
	{
		UserWidgetInterface->RequestReturnToParentWidget();
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Unable to Return from OptionsMenu Widget")) }
}


////////////////////////////////////////////////////////////////////////////////
// Button Hover

void UOptionsMenuWidget::ButtonApplyOnHover()
{
	Button_Apply->SetFocusToButton();
}

void UOptionsMenuWidget::ButtonResetOnHover()
{
	Button_Reset->SetFocusToButton();
}

void UOptionsMenuWidget::ButtonBackOnHover()
{
	Button_Back->SetFocusToButton();
}


////////////////////////////////////////////////////////////////////////////////
// Inputs

void UOptionsMenuWidget::BackInput()
{
	ReturnToPrevious();
}

void UOptionsMenuWidget::CloseMenuInput()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for OptionsMenu Widget")) return; }
	MenuInterface->ResumeGame();
}

