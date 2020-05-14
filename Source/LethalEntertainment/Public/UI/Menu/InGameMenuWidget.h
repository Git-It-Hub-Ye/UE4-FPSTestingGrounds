// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "InGameMenuWidget.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
class UControlsWidget;
class UMenuButtonsWidget;

/**
 * In game menu, can restart game or return to main menu from here
 */
UCLASS()
class LETHALENTERTAINMENT_API UInGameMenuWidget : public UMenuWidget
{
	GENERATED_BODY()
	
private:

	////////////////////////////////////////////////////////////////////////////////
	// Pause Panel

	/** Switches between different menu layouts */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher * WidgetSwitcher;

	/** First panel that will be visible to user */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * PausePanel;

	/** Allows user to resume game */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Resume;

	/** Allows user to open controls panel */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Controls;

	/** Allows user to restart game */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Restart;

	/** Allows user to return to main menu */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_MainMenu;

	/** True if not on Pause Panel */
	bool bIsAdditionalPanelOpen = false;

	/** True if viewing controls */
	bool bIsControlsPanelOpen = false;


	////////////////////////////////////////////////////////////////////////////////
	// Controls Panel

	/** Controls panel */
	UPROPERTY(meta = (BindWidget))
	UControlsWidget * ControlsPanel;


	////////////////////////////////////////////////////////////////////////////////
	// Restart Panel
	
	/** Restart panel */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * RestartPanel;

	/** Allows user to restart game */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_ConfirmRestart;

	/** Allows user to stay in game */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_CancelRestart;


	////////////////////////////////////////////////////////////////////////////////
	// Return to Main Menu Panel

	/** Return to main menu panel */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * ReturnMenuPanel;

	/** Allows user to return to main menu */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_ConfirmReturn;

	/** Allows user to stay in game */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_CancelReturn;

protected:
	/** Setup widget input functions */
	virtual bool Initialize() override;

	/** Updates widget anytime it is constructed or edited */
	virtual void NativePreConstruct() override;

	/** Any behavior after Super is carried out after construction */
	virtual void NativeConstruct() override;


	////////////////////////////////////////////////////////////////////////////////
	// IUserWidgetInterface Functions (Called by child widgets)

	/** Switches widget back to in game menu from child C++ widget */
	virtual void RequestReturnToParentWidget() override;

	
private:

	////////////////////////////////////////////////////////////////////////////////
	// Button Clicked

	/** Closes in game menu */
	UFUNCTION()
	void ResumeGame();

	/** Opens controls panel */
	UFUNCTION()
	void ViewControls();

	/** Opens restart menu */
	UFUNCTION()
	void WantsToRestart();

	/** Restarts game */
	UFUNCTION()
	void RestartGame();

	/** Opens return menu */
	UFUNCTION()
	void WantsToReturn();

	/** Returns back to main menu */
	UFUNCTION()
	void ReturnToMainMenu();

	/** Returns back to in game menu */
	UFUNCTION()
	void ReturnToInGameMenu();


	////////////////////////////////////////////////////////////////////////////////
	// Button Hover

	/** Sets focus on Button_Resume */
	UFUNCTION()
	void ButtonResumeOnHover();

	/** Sets focus on Button_Controls */
	UFUNCTION()
	void ButtonControlsOnHover();

	/** Sets focus on Button_Restart */
	UFUNCTION()
	void ButtonRestartOnHover();

	/** Sets focus on Button_MainMenu */
	UFUNCTION()
	void ButtonMainMenuOnHover();

	/** Sets focus on Button_ConfirmRestart */
	UFUNCTION()
	void ButtonConfirmRestartOnHover();

	/** Sets focus on Button_CancelRestart */
	UFUNCTION()
	void ButtonCancelRestartOnHover();

	/** Sets focus on Button_ConfirmReturn */
	UFUNCTION()
	void ButtonConfirmReturnOnHover();

	/** Sets focus on Button_CancelReturn */
	UFUNCTION()
	void ButtonCancelReturnOnHover();


	////////////////////////////////////////////////////////////////////////////////
	// Inputs

	/** Returns back to previous menu or closes menu */
	virtual void EscInput() override;

	/** Returns back to previous menu */
	virtual void BackInput() override;

	/** Instantly closes menu */
	virtual void CloseMenuInput() override;
	
};
