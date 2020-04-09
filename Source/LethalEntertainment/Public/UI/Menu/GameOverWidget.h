// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "GameOverWidget.generated.h"

class UButton;
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

	/** Allows user to restart game */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_Restart;

	/** Allows user to return to main menu */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_MainMenu;


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

private:
	virtual bool Initialize() override;

private:
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
	
};
