// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "MenuButtonsWidget.h"
#include "Components/TextBlock.h"
#include "IconsWidget.h"
#include "ScrollBoxWidget.h"
#include "ControlsWidget.h"

#define LOCTEXT_NAMESPACE "Controls"

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
	else { UE_LOG(LogTemp, Warning, TEXT("Button_Back is missing from Controls Widget")) return false; }

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
}


////////////////////////////////////////////////////////////////////////////////
// Setup

void UControlsWidget::SetScrollBoxInterface()
{
	if (ScrollBox_MouseKey) 
	{
		ScrollBox_MouseKey->OnWidgetFocused.AddUniqueDynamic(this, &UControlsWidget::SetScrollBoxFocusedWidget);
		ScrollBox_MouseKey->SetUserWidgetInterface(this);
		ScrollBox_MouseKey->SetSizeYOfScrollBox(ScrollBoxData.SizeY_ScrollBox_MouseKey);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ScrollBox_MouseKey missing from Controls Widget")) }

	if (ScrollBox_PS) 
	{ 
		ScrollBox_PS->OnWidgetFocused.AddUniqueDynamic(this, &UControlsWidget::SetScrollBoxFocusedWidget);
		ScrollBox_PS->SetUserWidgetInterface(this);
		ScrollBox_PS->SetSizeYOfScrollBox(ScrollBoxData.SizeY_ScrollBox_PS);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ScrollBox_PS missing from Controls Widget")) }

	if (ScrollBox_XB) 
	{
		ScrollBox_XB->OnWidgetFocused.AddUniqueDynamic(this, &UControlsWidget::SetScrollBoxFocusedWidget);
		ScrollBox_XB->SetUserWidgetInterface(this);
		ScrollBox_XB->SetSizeYOfScrollBox(ScrollBoxData.SizeY_ScrollBox_XB);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ScrollBox_XB missing from Controls Widget")) }
}

void UControlsWidget::SetUserWidgetInterface(IUserWidgetInterface * UserWidgetInt)
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

void UControlsWidget::SetScrollBoxType(EControlType IconControlType)
{
	Old_ControlType = IconControlType;
	Current_ControlType = IconControlType;

	if (Current_ControlType == EControlType::MouseAndKeyboard)
	{
		ShowMouseKeyInputs();
	}
	else if (Current_ControlType == EControlType::PlaystationController)
	{
		ShowPSControls();
	}
	else if (Current_ControlType == EControlType::XboxController)
	{
		ShowXBControls();
	}
}


////////////////////////////////////////////////////////////////////////////////
// Button Clicked

void UControlsWidget::SelectControlType()
{
	Button_ControlType->PlayPressedSound();

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
	Button_MouseKey->PlayPressedSound();

	Name_CurrentControlType = *Button_MouseKey->GetName();
	ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
	
	SetScrollMenu(ScrollBox_MouseKey);

	Button_ControlType->GetTextToCustomise()->SetText(LOCTEXT("Control Type", "Mouse & Keyboard"));
	SetControlsIcon(Icon_Key);
	Current_ControlType = EControlType::MouseAndKeyboard;
}

void UControlsWidget::ShowPSControls()
{
	Button_PS->PlayPressedSound();

	Name_CurrentControlType = *Button_PS->GetName();
	ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
	
	SetScrollMenu(ScrollBox_PS);

	Button_ControlType->GetTextToCustomise()->SetText(LOCTEXT("Control Type", "Playstation Controller"));
	SetControlsIcon(Icon_PS);
	Current_ControlType = EControlType::PlaystationController;
}

void UControlsWidget::ShowXBControls()
{
	Button_XB->PlayPressedSound();

	Name_CurrentControlType = *Button_XB->GetName();
	ConTypePanel->SetVisibility(ESlateVisibility::Hidden);

	SetScrollMenu(ScrollBox_XB);

	Button_ControlType->GetTextToCustomise()->SetText(LOCTEXT("Control Type", "Xbox Controller"));
	SetControlsIcon(Icon_XB);
	Current_ControlType = EControlType::XboxController;
}

void UControlsWidget::ReturnToPrevious()
{
	Button_Back->PlayPressedSound();

	if (ConTypePanel && ConTypePanel->GetVisibility() == ESlateVisibility::Visible)
	{
		ConTypePanel->SetVisibility(ESlateVisibility::Hidden);
		SetFocus();
	}
	else if (UserWidgetInterface)
	{
		OnControlTypeSet.Broadcast(Current_ControlType);
		SaveControlType();
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
	if (!GetCurrentScrollBox()) { UE_LOG(LogTemp, Warning, TEXT("Unable to Focus on ScrollBoxWidget from Controls Widget")) return; }
	GetCurrentScrollBox()->SetKeyboardFocus();
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

void UControlsWidget::SetScrollBoxFocusedWidget(UWidget * Widget)
{
	SetCurrentFocusedWidgetName(Widget);
	if (ConTypePanel && ConTypePanel->GetVisibility() == ESlateVisibility::Visible) { ConTypePanel->SetVisibility(ESlateVisibility::Hidden); }
}

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
// Inputs

void UControlsWidget::BackInput()
{
	ReturnToPrevious();
}

void UControlsWidget::CloseMenuInput()
{
	if (!MenuInterface) { UE_LOG(LogTemp, Warning, TEXT("No MenuInterface for Controls Widget")) return; }
	MenuInterface->ResumeGame();
}


////////////////////////////////////////////////////////////////////////////////
// Menu data

UScrollBoxWidget * UControlsWidget::GetCurrentScrollBox()
{
	UScrollBoxWidget * CurrentScrollBox = WidgetSwitcher->GetActiveWidget() ? Cast<UScrollBoxWidget>(WidgetSwitcher->GetActiveWidget()) : nullptr;
	return CurrentScrollBox;
}

#undef LOCTEXT_NAMESPACE

