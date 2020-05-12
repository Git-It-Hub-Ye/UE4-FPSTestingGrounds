// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "MainMenuWidget.generated.h"

class UMenuButtonsWidget;
class UPanelWidget;
class UWidgetSwitcher;
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

	/** Allows user to start game */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Play;

	/** Allows user to open controls panel */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Controls;

	/** Opens quit panel to confirm quit game */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Quit;


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
	void OnNavigatedToButtonPlay();

	/** Called from a Blueprint Widget's child widgets navigation (Used for custom navigation) */
	UFUNCTION(BlueprintCallable)
	void OnNavigatedToButtonControls();

	/** Called from a Blueprint Widget's child widgets navigation (Used for custom navigation) */
	UFUNCTION(BlueprintCallable)
	void OnNavigatedToButtonQuit();

};
