// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UPanelWidget;
class UWidgetSwitcher;

UCLASS()
class LETHALENTERTAINMENT_API UMainMenuWidget : public UMenuWidget
{
	GENERATED_BODY()
	
private:

	////////////////////////////////////////////////////////////////////////////////
	// Main menu

	/** Switches between different menu layouts */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher * WidgetSwitcher;

	/** First panel that will be visible to user */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * MainMenu;

	/** Allows user to start game */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_Play;

	/** Allows user return to open controls panel */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_Controls;

	/** Opens quit panel to confirm quit game */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_Quit;


	////////////////////////////////////////////////////////////////////////////////
	// Quit Menu

	/** Quit Panel */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * QuitMenu;

	/** Allows user quit game */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_ConfirmQuit;

	/** Allows user to return to main menu */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_CancelQuit;


	////////////////////////////////////////////////////////////////////////////////
	// Controls panel

	/** First panel that will be visible to user */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * ControlsPanel;

	/** Allows user return to previous menu */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_Return;

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void PlayGame();

	UFUNCTION()
	void WantsToQuit();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void CancelQuit();

	UFUNCTION()
	void ViewControls();

	UFUNCTION()
	void ReturnToPrevious();

};
