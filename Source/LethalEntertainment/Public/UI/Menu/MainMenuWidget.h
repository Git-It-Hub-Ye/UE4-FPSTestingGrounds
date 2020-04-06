// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "UserWidgetInterface.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UPanelWidget;
class UWidgetSwitcher;
class UControlsWidget;

UCLASS()
class LETHALENTERTAINMENT_API UMainMenuWidget : public UMenuWidget, public IUserWidgetInterface
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

	/** Controls Panel */
	UPROPERTY(meta = (BindWidget))
	UControlsWidget * ControlsPanel;

protected:
	virtual bool Initialize() override;

	/** Updates widget anytime it is constructed or edited */
	virtual void NativePreConstruct() override;

	/** Switches widget back to main menu from child C++ widget (Called through IUserWidgetInterface) */
	virtual void RequestReturnToParentWidget() override;

private:
	/** Starts game */
	UFUNCTION()
	void PlayGame();

	/** Opens quit menu */
	UFUNCTION()
	void WantsToQuit();

	/** Quits game */
	UFUNCTION()
	void QuitGame();

	/** Opens controls panel */
	UFUNCTION()
	void ViewControls();

	/** Returns back to main menu */
	UFUNCTION()
	void ReturnToMainMenu();

	/** Shows or hides quit button */
	void ShowOrHideQuitButton(bool bShowButton);

};
