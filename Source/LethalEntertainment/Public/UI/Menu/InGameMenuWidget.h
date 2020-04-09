// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "UserWidgetInterface.h"
#include "InGameMenuWidget.generated.h"

class UButton;
class UPanelWidget;
class UWidgetSwitcher;
class UControlsWidget;

/**
 * In game menu, can restart game or return to main menu from here
 */
UCLASS()
class LETHALENTERTAINMENT_API UInGameMenuWidget : public UMenuWidget, public IUserWidgetInterface
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
	UButton * Button_Resume;

	/** Allows user to open controls panel */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_Controls;

	/** Allows user to restart game */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_Restart;

	/** Allows user to return to main menu */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_MainMenu;


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
	UButton * Button_ConfirmRestart;

	/** Allows user to stay in game */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_CancelRestart;


	////////////////////////////////////////////////////////////////////////////////
	// Return to Main Menu Panel

	/** Return to main menu panel */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * ReturnMenuPanel;

	/** Allows user to return to main menu */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_ConfirmReturn;

	/** Allows user to stay in game */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_CancelReturn;

protected:
	virtual bool Initialize() override;

	/** Updates widget anytime it is constructed or edited */
	virtual void NativePreConstruct() override;

	/** Switches widget back to in game menu from child C++ widget (Called through IUserWidgetInterface) */
	virtual void RequestReturnToParentWidget() override;

private:
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
	
};
