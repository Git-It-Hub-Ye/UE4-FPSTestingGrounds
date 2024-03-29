// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "GameOverWidget.generated.h"

class UMenuButtonsWidget;
class UPanelWidget;
class UWidgetSwitcher;

/**
 * Game Over Menu, can restart game or return to main menu from here
 */
UCLASS()
class LETHALENTERTAINMENT_API UGameOverWidget : public UMenuWidget
{
	GENERATED_BODY()
	
private:

	////////////////////////////////////////////////////////////////////////////////
	// Menu Panel

	/** Switches between different menu layouts */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher * WidgetSwitcher;

	/** First panel that will be visible to user */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * MenuPanel;

	/** Loading panel that will be visible to user */
	UPROPERTY(meta = (BindWidget))
	UUserWidget * LoadingScreen;

	/** Allows user to restart game */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Restart;

	/** Allows user to return to main menu */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_MainMenu;


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

	/** Any behavior after Super is carried out after construction */
	virtual void NativeConstruct() override;

private:
	////////////////////////////////////////////////////////////////////////////////
	// Button Clicked

	/** Restarts game */
	UFUNCTION()
	void RestartGame();

	/** Opens return menu */
	UFUNCTION()
	void WantsToReturn();

	/** Stays in game over menu */
	UFUNCTION()
	void CancelReturn();

	/** Returns back to main menu */
	UFUNCTION()
	void ReturnToMainMenu();


	////////////////////////////////////////////////////////////////////////////////
	// Button Hover

	/** Sets focus on Button_Resume */
	UFUNCTION()
	void ButtonRestartOnHover();

	/** Sets focus on Button_MainMenu */
	UFUNCTION()
	void ButtonMainMenuOnHover();

	/** Sets focus on Button_ConfirmReturn */
	UFUNCTION()
	void ButtonConfirmReturnOnHover();

	/** Sets focus on Button_CancelReturn */
	UFUNCTION()
	void ButtonCancelReturnOnHover();


	////////////////////////////////////////////////////////////////////////////////
	// Inputs

	/** Returns back to previous menu */
	virtual void EscInput() override { BackInput(); }

	/** Returns back to previous menu */
	virtual void BackInput() override;
	
};
