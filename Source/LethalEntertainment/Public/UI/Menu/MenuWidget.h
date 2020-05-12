// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "UserWidgetInterface.h"
#include "MenuWidget.generated.h"


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

public:

	////////////////////////////////////////////////////////////////////////////////
	// Setup

	/** Sets input mode */
	virtual void Setup();

	/** Remove widget from veiwport and return input to controller */
	void TearDown();

	/** Sets menu interface variable */
	void SetMenuInterface(IMenuInterface * MenuInt);


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
};
