// Copyright 2018 - 2021 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "MenuButtonsWidget.h"
#include "Components/TextBlock.h"
#include "SliderWidget.h"
#include "IconsWidget.h"
#include "OptionsMenuWidget.h"


#define LOCTEXT_NAMESPACE "Options"


bool UOptionsMenuWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }

	this->SetVisibility(ESlateVisibility::Visible);

	if (Button_InvertY && Button_InvertY->GetButton())
	{
		Button_InvertY->GetButton()->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ToggleInvertY);
		Button_InvertY->GetButton()->OnHovered.AddDynamic(this, &UOptionsMenuWidget::ButtonInvertYOnHover);
		Button_InvertY->OnWidgetFocused.AddUniqueDynamic(this, &UOptionsMenuWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_InvertY is missing from OptionsMenu Widget")) return false; }

	if (Button_Vibration && Button_Vibration->GetButton())
	{
		Button_Vibration->GetButton()->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ToggleVibration);
		Button_Vibration->GetButton()->OnHovered.AddDynamic(this, &UOptionsMenuWidget::ButtonVibrationOnHover);
		Button_Vibration->OnWidgetFocused.AddUniqueDynamic(this, &UOptionsMenuWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Vibration is missing from OptionsMenu Widget")) return false; }

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

void UOptionsMenuWidget::SetUserWidgetInterface(IUserWidgetInterface * UserWidgetInt)
{
	UserWidgetInterface = UserWidgetInt;
}

void UOptionsMenuWidget::SetInitialValues()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("MenuInterface is missing from OptionsMenu Widget")) return; }
	MenuInterface->GetCurrentUserValues(Current_MouseSens, Current_ADS_MouseSens, Current_ConSens, Current_ADS_ConSens, Current_InvertY, Current_Vibration);
	MenuInterface->GetDefaultUserValues(Default_MouseSens, Default_ADS_MouseSens, Default_ConSens, Default_ADS_ConSens, Default_InvertY, Default_Vibration);
	SetUserSettingsValue(Current_MouseSens, Current_ADS_MouseSens, Current_ConSens, Current_ADS_ConSens, Current_InvertY, Current_Vibration);
}

void UOptionsMenuWidget::SetFocus(EControlType IconControlType)
{
	Current_ControlType = IconControlType;

	if (Current_ControlType == EControlType::MouseAndKeyboard)
	{
		SetControlsIcon(Icon_Key);
	}
	else if (Current_ControlType == EControlType::PlaystationController)
	{
		SetControlsIcon(Icon_PS);
	}
	else if (Current_ControlType == EControlType::XboxController)
	{
		SetControlsIcon(Icon_XB);
	}

	if (Button_InvertY)
	{
		Button_InvertY->SetFocusToButton();
	}
}

void UOptionsMenuWidget::SetUserSettingsValue(float MouseSens, float ADS_MouseSens, float ConSens, float ADS_ConSens, bool Invert_Y, bool Vibration)
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

	if (Invert_Y && Button_InvertY)
	{
		Button_InvertY->GetTextToCustomise()->SetText(LOCTEXT("Invert Y", "On"));
	}
	else if (Button_InvertY)
	{
		Button_InvertY->GetTextToCustomise()->SetText(LOCTEXT("Invert Y", "Off"));
	}

	Current_InvertY = Invert_Y;

	if (Vibration && Button_Vibration)
	{
		Button_Vibration->GetTextToCustomise()->SetText(LOCTEXT("Vibration", "On"));
	}
	else if (Button_Vibration)
	{
		Button_Vibration->GetTextToCustomise()->SetText(LOCTEXT("Vibration", "Off"));
	}

	Current_Vibration = Vibration;
}


////////////////////////////////////////////////////////////////////////////////
// Display

void UOptionsMenuWidget::SetControlsIcon(UIconsWidget * Icon)
{
	if (!WidgetSwitcherIcons || !Icon) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch Controls Icon in Controls Widget")) return; }
	WidgetSwitcherIcons->SetActiveWidget(Icon);
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UOptionsMenuWidget::ToggleInvertY()
{
	Button_InvertY->PlayPressedSound();
	ButtonInvertYOnHover();

	if (Current_InvertY)
	{
		Current_InvertY = false;
		Button_InvertY->GetTextToCustomise()->SetText(LOCTEXT("Invert Y", "Off"));
	}
	else
	{
		Current_InvertY = true;
		Button_InvertY->GetTextToCustomise()->SetText(LOCTEXT("Invert Y", "On"));
	}
}

void UOptionsMenuWidget::ToggleVibration()
{
	Button_Vibration->PlayPressedSound();
	ButtonVibrationOnHover();

	if (Current_Vibration)
	{
		Current_Vibration = false;
		Button_Vibration->GetTextToCustomise()->SetText(LOCTEXT("Vibration", "Off"));
	}
	else
	{
		Current_Vibration = true;
		Button_Vibration->GetTextToCustomise()->SetText(LOCTEXT("Vibration", "On"));
	}
}

void UOptionsMenuWidget::ApplyChanges()
{
	Button_Apply->PlayPressedSound();

	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("MenuInterface is missing from OptionsMenu Widget")) return; }
	if (Slider_MouseSens)		{ Current_MouseSens		=	Slider_MouseSens->GetCurrentValue();	 }
	if (Slider_ADS_MouseSens)	{ Current_ADS_MouseSens =	Slider_ADS_MouseSens->GetCurrentValue(); }
	if (Slider_ConSens)			{ Current_ConSens		=	Slider_ConSens->GetCurrentValue();		 }
	if (Slider_ADS_ConSens)		{ Current_ADS_ConSens	=	Slider_ADS_ConSens->GetCurrentValue();	 }

	MenuInterface->SetNewUserSettings(Current_MouseSens, Current_ADS_MouseSens, Current_ConSens, Current_ADS_ConSens, Current_InvertY, Current_Vibration);
}

void UOptionsMenuWidget::ResetToDefaults()
{
	Button_Reset->PlayPressedSound();

	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("MenuInterface is missing from OptionsMenu Widget")) return; }
	MenuInterface->SetNewUserSettings(Default_MouseSens, Default_ADS_MouseSens, Default_ConSens, Default_ADS_ConSens, Default_InvertY, Default_Vibration);
	SetUserSettingsValue(Default_MouseSens, Default_ADS_MouseSens, Default_ConSens, Default_ADS_ConSens, Default_InvertY, Default_Vibration);
}

void UOptionsMenuWidget::ReturnToPrevious()
{
	Button_Back->PlayPressedSound();

	if (UserWidgetInterface)
	{
		UserWidgetInterface->RequestReturnToParentWidget();
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Unable to Return from OptionsMenu Widget")) }
}


////////////////////////////////////////////////////////////////////////////////
// Button Hover

void UOptionsMenuWidget::ButtonInvertYOnHover()
{
	Button_InvertY->SetFocusToButton();
}

void UOptionsMenuWidget::ButtonVibrationOnHover()
{
	Button_Vibration->SetFocusToButton();
}

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

#undef LOCTEXT_NAMESPACE

