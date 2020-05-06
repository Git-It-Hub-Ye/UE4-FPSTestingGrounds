// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "MenuButtonsWidget.h"
#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface * MenuInt)
{
	MenuInterface = MenuInt;
}

void UMenuWidget::Setup()
{
	this->AddToViewport();

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

FReply UMenuWidget::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return FReply::Handled();
}

APlayerController * UMenuWidget::GetLocalPlayerController()
{
	APlayerController * PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	return PC;
}

void UMenuWidget::SetWidgetToFocus(FName Name_ButtonWidget)
{
	UMenuButtonsWidget * Button = GetWidgetFromName(Name_ButtonWidget) ? Cast<UMenuButtonsWidget>(GetWidgetFromName(Name_ButtonWidget)) : nullptr;
	if (!Button) { UE_LOG(LogTemp, Warning, TEXT("Unable to focus on button Widget with this name in UMenuWidget")) return; }
	Button->SetFocusToButton();
}

