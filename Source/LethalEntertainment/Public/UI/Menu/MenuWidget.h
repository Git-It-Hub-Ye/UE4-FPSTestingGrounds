// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "UserWidgetInterface.h"
#include "MenuWidget.generated.h"

UENUM()
enum class EControlType : uint8 {
	MouseAndKeyboard = 0,
	PlaystationController = 1,
	XboxController = 2
};

UCLASS()
class LETHALENTERTAINMENT_API UMenuWidget : public UUserWidget, public IUserWidgetInterface
{
	GENERATED_BODY()
	
protected:
	////////////////////////////////////////////////////////////////////////////////
	// Menu Data

	/** Calls interface functions (These functions should be overriden by game instance) */
	IMenuInterface * MenuInterface;

	/** Stores name of last button clicked (Used to set button to focus on when returning to previous menu layouts) */
	FName Name_LastButton = "None";

	/** Stores name of current widget with user focus (Used to reset focus on widget, whenever focus is lost) */
	FName Name_CurrentFocusedWidget = "None";

	/** Current control type */
	EControlType Current_ControlType;

	/** Current control type */
	EControlType Old_ControlType;

public:

	////////////////////////////////////////////////////////////////////////////////
	// Setup

	/** Sets input mode */
	virtual void Setup();

	/** Remove widget from veiwport and return input to controller */
	void TearDown();

	/** Sets menu interface variable */
	void SetMenuInterface(IMenuInterface * MenuInt);

	/** Sets current saved control type */
	void SetInitialControlType(EControlType NewControlType);

protected:
	/** Called when level is removed and will remove widget from viewport */
	virtual void OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld) override;


	////////////////////////////////////////////////////////////////////////////////
	// IUserWidgetInterface Functions (Called by child widgets)

	/** For returning back to parent widgets */
	UFUNCTION()
	virtual void RequestReturnToParentWidget() override { return; }

	/** Switch focus to parent widget above (Used for navigation from child to parent in same view panel) */
	UFUNCTION()
	virtual void OnNavUpToParent() override { return; }

	/** Switch focus to parent widget below (Used for navigation from child to parent in same view panel) */
	UFUNCTION()
	virtual void OnNavDownToParent() override { return; }


	////////////////////////////////////////////////////////////////////////////////
	// Inputs

	/** Listens for UP inputs */
	virtual FReply NativeOnKeyUp(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent) override;

	/** Returns back to previous menu or closes menu */
	UFUNCTION()
	virtual void EscInput() { return; }

	/** Returns back to previous menu */
	UFUNCTION()
	virtual void BackInput() { return; }

	/** Instantly closes menu */
	UFUNCTION()
	virtual void CloseMenuInput() { return; }


	////////////////////////////////////////////////////////////////////////////////
	// Focus

	/** Resets focus back to Name_CurrentFocusedWidget (Widgets base must be set to visible). When widget receives focus, this means a child widget (eg Button) has lost focus */
	virtual FReply NativeOnFocusReceived(const FGeometry & InGeometry, const FFocusEvent & InFocusEvent) override;

	/** Tracks mouse clicks. (Used for UMenuButtonsWidget to track when focus is lost due to random mouse click) */
	virtual FReply NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent) override;

	/** Default behaviour to reassign focus to a widget */
	UFUNCTION()
	virtual void ReassignFocus() { SetWidgetToFocus(Name_CurrentFocusedWidget); }

	/** Sets a button widget to focus on for player controller */
	void SetWidgetToFocus(FName Name_Widget);

	/** Sets Name_CurrentFocusedWidget */
	UFUNCTION()
	void SetCurrentFocusedWidgetName(UWidget * CurrentWidget);


	////////////////////////////////////////////////////////////////////////////////
	// Menu Data

	/** Gets the player controller for this widget */
	APlayerController * GetLocalPlayerController();

	/** Sets control type enum */
	UFUNCTION()
	void SetControlType(EControlType NewControlType);

	/** If needed tells game to save new control type */
	void SaveControlType();

};
