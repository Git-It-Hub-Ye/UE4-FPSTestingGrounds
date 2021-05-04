// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "MainMenuWidget.generated.h"

class UMenuButtonsWidget;
class UPanelWidget;
class UWidgetSwitcher;
class UOptionsMenuWidget;
class UControlsWidget;

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

	/** Loading panel that will be visible to user */
	UPROPERTY(meta = (BindWidget))
	UUserWidget * LoadingScreen;

	/** Allows user to start game */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Play;

	/** Allows user to open options panel */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Options;

	/** Allows user to open controls panel */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Controls;

	/** Opens quit panel to confirm quit game */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Quit;

	/** Name of button to navigate back to from quit button */
	FName Name_NavToButton;

	/** True if not on Main Menu */
	bool bIsAdditionalPanelOpen = false;


	////////////////////////////////////////////////////////////////////////////////
	// Quit Menu

	/** Quit Panel */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * QuitMenu;

	/** Allows user quit game */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_ConfirmQuit;

	/** Allows user to return to main menu */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_CancelQuit;


	////////////////////////////////////////////////////////////////////////////////
	// Options Panel

	/** Options panel */
	UPROPERTY(meta = (BindWidget))
	UOptionsMenuWidget * OptionsPanel;


	////////////////////////////////////////////////////////////////////////////////
	// Controls panel

	/** Controls Panel */
	UPROPERTY(meta = (BindWidget))
	UControlsWidget * ControlsPanel;

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

	/** Starts game */
	UFUNCTION()
	void PlayGame();

	/** Opens quit menu */
	UFUNCTION()
	void WantsToQuit();

	/** Quits game */
	UFUNCTION()
	void QuitGame();

	/** Opens options panel */
	UFUNCTION()
	void ViewOptions();

	/** Opens controls panel */
	UFUNCTION()
	void ViewControls();

	/** Returns back to main menu */
	UFUNCTION()
	void ReturnToMainMenu();


	////////////////////////////////////////////////////////////////////////////////
	// Button Hover

	/** Sets focus on Button_Play */
	UFUNCTION()
	void ButtonPlayOnHover();

	/** Sets focus on Button_Options */
	UFUNCTION()
	void ButtonOptionsOnHover();

	/** Sets focus on Button_Controls */
	UFUNCTION()
	void ButtonControlsOnHover();

	/** Sets focus on Button_Quit */
	UFUNCTION()
	void ButtonOuitOnHover();

	/** Sets focus on Button_ConfirmQuit */
	UFUNCTION()
	void ButtonConfirmQuitOnHover();

	/** Sets focus on Button_CancelQuit */
	UFUNCTION()
	void ButtonCancelQuitOnHover();


	////////////////////////////////////////////////////////////////////////////////
	// Navigation Functions

	/** Called from a Blueprint Widget's child widgets navigation (Used for custom navigation) */
	UFUNCTION(BlueprintCallable)
	void OnNavigateToButton();

	/** Called from a Blueprint Widget's child widgets navigation (Used for custom navigation) */
	UFUNCTION(BlueprintCallable)
	void OnNavigatedToButtonQuit();


	////////////////////////////////////////////////////////////////////////////////
	// Focus

	/** Set current focused widget and Name_NavToButton */
	UFUNCTION()
	void SetFocusedWidgetName(UWidget * Widget);


	////////////////////////////////////////////////////////////////////////////////
	// Inputs

	/** Returns back to previous menu or closes menu */
	virtual void EscInput() override;

	/** Returns back to previous menu */
	virtual void BackInput() override { EscInput(); }

};
