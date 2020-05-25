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

	if (Slider_ADS_MouseSens)
	{
		Slider_ADS_MouseSens->OnWidgetFocused.AddUniqueDynamic(this, &UOptionsMenuWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Slider_ADS_MouseSens is missing from OptionsMenu Widget")) }

	if (Slider_ConSens)
	{
		Slider_ConSens->OnWidgetFocused.AddUniqueDynamic(this, &UOptionsMenuWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Slider_ConSens is missing from OptionsMenu Widget")) }

	if (!Slider_ADS_ConSens) { UE_LOG(LogTemp, Warning, TEXT("Slider_ADS_ConSens is missing from OptionsMenu Widget")) return; }
	Slider_ADS_ConSens->OnWidgetFocused.AddUniqueDynamic(this, &UOptionsMenuWidget::SetCurrentFocusedWidgetName);
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
	MenuInterface->GetCurrentUserValues(Current_MouseSens, Current_ADS_MouseSens, Current_ConSens, Current_ADS_ConSens);
	MenuInterface->GetDefaultUserValues(Default_MouseSens, Default_ADS_MouseSens, Default_ConSens, Default_ADS_ConSens);
	SetUserSettingsValue(Current_MouseSens, Current_ADS_MouseSens, Current_ConSens, Current_ADS_ConSens);
}

void UOptionsMenuWidget::SetFocus()
{
	if (Slider_MouseSens)
	{
		Slider_MouseSens->SetKeyboardFocus();
	}
}

void UOptionsMenuWidget::SetUserSettingsValue(float MouseSens, float ADS_MouseSens, float ConSens, float ADS_ConSens)
{
	if (Slider_MouseSens)
	{
		Slider_MouseSens->SetInitialValue(MouseSens);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("No Slider_MouseSens is missing from OptionsMenu Widget")) }

	if (Slider_ADS_MouseSens)
	{
		Slider_ADS_MouseSens->SetInitialValue(ADS_MouseSens);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("No Slider_ADS_MouseSens is missing from OptionsMenu Widget")) }

	if (Slider_ConSens)
	{
		Slider_ConSens->SetInitialValue(ConSens);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("No Slider_ConSens is missing from OptionsMenu Widget")) }

	if (Slider_ADS_ConSens)
	{
		Slider_ADS_ConSens->SetInitialValue(ADS_ConSens);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("No Slider_ADS_ConSens is missing from OptionsMenu Widget")) }
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UOptionsMenuWidget::ApplyChanges()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("MenuInterface is missing from OptionsMenu Widget")) return; }
	if (Slider_MouseSens)		{ Current_MouseSens		=	Slider_MouseSens->GetCurrentValue();	 }
	if (Slider_ADS_MouseSens)	{ Current_ADS_MouseSens =	Slider_ADS_MouseSens->GetCurrentValue(); }
	if (Slider_ConSens)			{ Current_ConSens		=	Slider_ConSens->GetCurrentValue();		 }
	if (Slider_ADS_ConSens)		{ Current_ADS_ConSens	=	Slider_ADS_ConSens->GetCurrentValue();	 }

	MenuInterface->SetNewUserSettings(Current_MouseSens, Current_ADS_MouseSens, Current_ConSens, Current_ADS_ConSens);
}

void UOptionsMenuWidget::ResetToDefaults()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("MenuInterface is missing from OptionsMenu Widget")) return; }
	MenuInterface->SetNewUserSettings(Default_MouseSens, Default_ADS_MouseSens, Default_ConSens, Default_ADS_ConSens);
	SetUserSettingsValue(Default_MouseSens, Default_ADS_MouseSens, Default_ConSens, Default_ADS_ConSens);
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

