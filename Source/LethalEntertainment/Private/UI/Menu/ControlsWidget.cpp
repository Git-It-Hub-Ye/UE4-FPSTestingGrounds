// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "MenuButtonsWidget.h"
#include "Components/TextBlock.h"
#include "IconsWidget.h"
#include "ScrollBoxWidget.h"
#include "ControlsWidget.h"

#define LOCTEXT_NAMESPACE "PlayerHud"

bool UControlsWidget::Initialize()
{
	if (!Super::Initialize()) { return false; }
	
	// Ensure this widget can be clicked on to monitor user focus
	this->SetVisibility(ESlateVisibility::Visible);

	if (Button_Back && Button_Back->GetButton())
	{
		Button_Back->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::ReturnToPrevious);
		Button_Back->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonBackOnHover);
		Button_Back->OnWidgetFocused.AddUniqueDynamic(this, &UControlsWidget::SetCurrentFocusedWidgetName);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Return is missing from Controls Widget")) return false; }

	if (Button_ControlType && Button_ControlType->GetButton())
	{
		Button_ControlType->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::SelectControlType);
		Button_ControlType->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonControlTypeOnHover);
		Button_ControlType->OnWidgetFocused.AddUniqueDynamic(this, &UControlsWidget::SetCurrentFocusedWidgetName);

		if (Button_MouseKey && Button_MouseKey->GetButton())
		{
			Button_MouseKey->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::ShowMouseKeyInputs);
			Button_MouseKey->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonMouseKeyOnHover);
			Button_MouseKey->OnWidgetFocused.AddUniqueDynamic(this, &UControlsWidget::SetCurrentFocusedWidgetName);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_MouseKey is missing from Controls Widget")) }

		if (Button_PS && Button_PS->GetButton())
		{
			Button_PS->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::ShowPSControls);
			Button_PS->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonPSOnHover);
			Button_PS->OnWidgetFocused.AddUniqueDynamic(this, &UControlsWidget::SetCurrentFocusedWidgetName);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_PS is missing from Controls Widget")) }

		if (Button_XB && Button_XB->GetButton())
		{
			Button_XB->GetButton()->OnClicked.AddDynamic(this, &UControlsWidget::ShowXBControls);
			Button_XB->GetButton()->OnHovered.AddDynamic(this, &UControlsWidget::ButtonXBOnHover);
			Button_XB->OnWidgetFocused.AddUniqueDynamic(this, &UControlsWidget::SetCurrentFocusedWidgetName);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Button_XB is missing from Controls Widget")) }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Button_ControlType is missing from Controls Widget")) }

	return true;
}

void UControlsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConTypePanel)
	{
		ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ConTypePanel is missing from GameOverMenu Widget")) }
	SetScrollBoxInterface();
	SetControlTypeButton();
}


////////////////////////////////////////////////////////////////////////////////
// Setup

void UControlsWidget::SetScrollBoxInterface()
{
	if (ScrollBox_MouseKey) 
	{
		ScrollBox_MouseKey->OnWidgetFocused.AddUniqueDynamic(this, &UControlsWidget::SetCurrentFocusedWidgetName);
		ScrollBox_MouseKey->SetUserWidgetInterface(this);
		ScrollBox_MouseKey->SetSizeYOfScrollBox(ScrollBoxData.SizeY_ScrollBox_MouseKey);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ScrollBox_MouseKey missing from Controls Widget")) }

	if (ScrollBox_PS) 
	{ 
		ScrollBox_PS->OnWidgetFocused.AddUniqueDynamic(this, &UControlsWidget::SetCurrentFocusedWidgetName);
		ScrollBox_PS->SetUserWidgetInterface(this);
		ScrollBox_PS->SetSizeYOfScrollBox(ScrollBoxData.SizeY_ScrollBox_PS);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ScrollBox_PS missing from Controls Widget")) }

	if (ScrollBox_XB) 
	{
		ScrollBox_XB->OnWidgetFocused.AddUniqueDynamic(this, &UControlsWidget::SetCurrentFocusedWidgetName);
		ScrollBox_XB->SetUserWidgetInterface(this);
		ScrollBox_XB->SetSizeYOfScrollBox(ScrollBoxData.SizeY_ScrollBox_XB);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ScrollBox_XB missing from Controls Widget")) }
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
	if (Button_ControlType)
	{
		Button_ControlType->SetFocusToButton();
	} 
	else
	{ UE_LOG(LogTemp, Warning, TEXT("Button_MouseKey is missing from Controls Widget")) }
	
	if (!GetCurrentScrollBox()) { UE_LOG(LogTemp, Warning, TEXT("Scrollbox is missing from Controls Widget")) return; }
	GetCurrentScrollBox()->ResetOffset();
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UControlsWidget::SelectControlType()
{
	if (!ConTypePanel) { UE_LOG(LogTemp, Warning, TEXT("ConTypePanel is missing from Controls Widget")) return; }
	if (ConTypePanel->Visibility == ESlateVisibility::Visible)
	{
		ReturnToPrevious();
	}
	else
	{
		ConTypePanel->SetVisibility(ESlateVisibility::Visible);
		SetWidgetToFocus(Name_CurrentControlType);
	}
}

void UControlsWidget::ShowMouseKeyInputs()
{
	Name_CurrentControlType = *Button_MouseKey->GetName();
	ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
	
	SetScrollMenu(ScrollBox_MouseKey);

	Button_ControlType->GetTextToCustomise()->SetText(LOCTEXT("Control Type", "Mouse & Keyboard"));
	SetControlsIcon(Icon_Key);
}

void UControlsWidget::ShowPSControls()
{
	Name_CurrentControlType = *Button_PS->GetName();
	ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
	
	SetScrollMenu(ScrollBox_PS);

	Button_ControlType->GetTextToCustomise()->SetText(LOCTEXT("Control Type", "Playstation Controller"));
	SetControlsIcon(Icon_PS);
}

void UControlsWidget::ShowXBControls()
{
	Name_CurrentControlType = *Button_XB->GetName();
	ConTypePanel->SetVisibility(ESlateVisibility::Hidden);

	SetScrollMenu(ScrollBox_XB);

	Button_ControlType->GetTextToCustomise()->SetText(LOCTEXT("Control Type", "Xbox Controller"));
	SetControlsIcon(Icon_XB);
}

void UControlsWidget::ReturnToPrevious()
{
	if (ConTypePanel && ConTypePanel->GetVisibility() == ESlateVisibility::Visible)
	{
		ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
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
// Navigation Functions

void UControlsWidget::OnNavigatedToScrollMenu()
{
	if (!GetCurrentScrollBox() || !GetLocalPlayerController()) { UE_LOG(LogTemp, Warning, TEXT("Unable to SetUserFocus on ScrollBoxWidget from Controls Widget")) return; }
	GetCurrentScrollBox()->SetUserFocus(GetLocalPlayerController());
}

void UControlsWidget::OnNavUpToParent()
{
	if (!Button_ControlType) { UE_LOG(LogTemp, Warning, TEXT("Button_ControlType is missing from Controls Widget")) return; }
	ButtonControlTypeOnHover();
}

void UControlsWidget::OnNavDownToParent()
{
	if (!Button_Back) { UE_LOG(LogTemp, Warning, TEXT("Button_Back is missing from Controls Widget")) return; }
	ButtonBackOnHover();
}


////////////////////////////////////////////////////////////////////////////////
// Display

void UControlsWidget::SetScrollMenu(UScrollBoxWidget * ScrollBox)
{
	if (!WidgetSwitcher || !ScrollBox) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch Scroll Menu in Controls Widget")) return; }
	WidgetSwitcher->SetActiveWidget(ScrollBox);
	SetFocus();
}

void UControlsWidget::SetControlsIcon(UIconsWidget * Icon)
{
	if (!WidgetSwitcherIcons || !Icon) { UE_LOG(LogTemp, Warning, TEXT("Unable to switch Controls Icon in Controls Widget")) return; }
	WidgetSwitcherIcons->SetActiveWidget(Icon);
}


////////////////////////////////////////////////////////////////////////////////
// Focus

void UControlsWidget::ReassignFocus()
{
	if (ConTypePanel && ConTypePanel->GetVisibility() == ESlateVisibility::Visible)
	{
		ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
		SetFocus();
	}
	else
	{
		SetWidgetToFocus(Name_CurrentFocusedWidget);
	}
}


////////////////////////////////////////////////////////////////////////////////
// Menu data

UScrollBoxWidget * UControlsWidget::GetCurrentScrollBox()
{
	UScrollBoxWidget * CurrentScrollBox = WidgetSwitcher->GetActiveWidget() ? Cast<UScrollBoxWidget>(WidgetSwitcher->GetActiveWidget()) : nullptr;
	return CurrentScrollBox;
}

#undef LOCTEXT_NAMESPACE

