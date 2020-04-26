// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

UCLASS()
class LETHALENTERTAINMENT_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	/** Calls interface functions (These functions should be overriden by game instance) */
	IMenuInterface * MenuInterface;

	/** Stores name of last button clicked (Used to set button to focus on when returning to previous menu layouts) */
	FName Name_LastButton = "None";

	/** Stores name of current widget with user focus (Used to reset focus on widget, whenever focus is lost) */
	FName Name_CurrentFocusedWidget = "None";

public:
	/** Sets input mode */
	virtual void Setup();

	/** Remove widget from veiwport and return input to controller */
	void TearDown();

	/** Sets menu interface variable */
	void SetMenuInterface(IMenuInterface * MenuInt);

protected:
	virtual void OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld) override;

	/** Gets the player controller for this widget */
	APlayerController * GetLocalPlayerController();

	/** Sets a button widget to focus on for player controller */
	void SetWidgetToFocus(FName Name_ButtonWidget);
	
};
