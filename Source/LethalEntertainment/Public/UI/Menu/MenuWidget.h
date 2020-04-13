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

	/** Sets a widget to focus on for player controller */
	void SetWidgetToFocus(FName Name_Widget);
	
	
};
