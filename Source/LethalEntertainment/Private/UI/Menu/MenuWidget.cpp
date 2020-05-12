// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "MenuButtonsWidget.h"
#include "MenuWidget.h"


////////////////////////////////////////////////////////////////////////////////
// Setup

void UMenuWidget::SetMenuInterface(IMenuInterface * MenuInt)
{
	MenuInterface = MenuInt;
}

void UMenuWidget::Setup()
{
	this->AddToViewport();

	// Ensure this widget can be clicked on to monitor user focus
	this->SetVisibility(ESlateVisibility::Visible);

	APlayerController * PC = GetLocalPlayerController();
	if (!PC) { return; }

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
}

void UMenuWidget::TearDown()
{
	this->RemoveFromViewport();

	APlayerController * PC = GetLocalPlayerController();
	if (!PC) { return; }

	PC->SetInputMode(FInputModeGameOnly());
	PC->bShowMouseCursor = false;
}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	TearDown();
}


////////////////////////////////////////////////////////////////////////////////
// Focus

FReply UMenuWidget::NativeOnFocusReceived(const FGeometry & InGeometry, const FFocusEvent & InFocusEvent)
{
	Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	ReassignFocus();
	return FReply::Handled();
}

FReply UMenuWidget::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	return FReply::Handled();
}

void UMenuWidget::SetWidgetToFocus(FName Name_Widget)
{	
	UMenuButtonsWidget * Button = GetWidgetFromName(Name_Widget) ? Cast<UMenuButtonsWidget>(GetWidgetFromName(Name_Widget)) : nullptr;
	if (Button)
	{
		Button->SetFocusToButton();
		return;
	}

	UWidget * Widget = GetWidgetFromName(Name_Widget) ? Cast<UWidget>(GetWidgetFromName(Name_Widget)) : nullptr;
	if (!Widget) { UE_LOG(LogTemp, Warning, TEXT("Unable to focus on Widget with this name in UMenuWidget")) return; }
	Widget->SetKeyboardFocus();
}

void UMenuWidget::SetCurrentFocusedWidgetName(UWidget * CurrentWidget)
{
	if (!CurrentWidget) { UE_LOG(LogTemp, Warning, TEXT("No Current Focused Widget for UMenuWidget")) return; }
	Name_CurrentFocusedWidget = *CurrentWidget->GetName();
}


////////////////////////////////////////////////////////////////////////////////
// Menu Data

APlayerController * UMenuWidget::GetLocalPlayerController()
{
	APlayerController * PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	return PC;
}

