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
	if (Slider_MouseSens)
	{
		Slider_MouseSens->SetInitialValue(MenuInterface->GetMouseSensitivity());
	}
	else { UE_LOG(LogTemp, Warning, TEXT("No Slider_MouseSens is missing from OptionsMenu Widget")) }

	if (Slider_ConSens)
	{
		Slider_ConSens->SetInitialValue(MenuInterface->GetMouseSensitivity());
	}
	else { UE_LOG(LogTemp, Warning, TEXT("No Slider_ConSens is missing from OptionsMenu Widget")) }
}

void UOptionsMenuWidget::SetFocus()
{
	
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

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

void UOptionsMenuWidget::ButtonBackOnHover()
{
	Button_Back->SetFocusToButton();
}

