// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetInterface.h"
#include "ControlsWidget.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
class UButton;

/**
 * Displays mouse/key bindings and gamepad controls
 */
UCLASS()
class LETHALENTERTAINMENT_API UControlsWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	/** Calls interface functions (These functions should be overriden by parent widget) */
	IUserWidgetInterface * UserWidgetInterface;


	////////////////////////////////////////////////////////////////////////////////
	// Control Panel
	
	/** Switches between different menu layouts */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher * WidgetSwitcher;

	/** Controls Panel */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * ControlsPanel;

	/** Displays Mouse/Key bindings */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * MouseKeyPanel;

	/** Displays gamepad controls */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * GamePadPanel;

	/** Allows user to return to previous panel */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_MouseKey;

	/** Allows user to return to previous panel */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_Controller;

	/** Allows user to return to previous panel */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_Return;


	////////////////////////////////////////////////////////////////////////////////
	// GamePad Panel

	/** Switches between different menu layouts */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher * WidgetSwitcher_Gamepad;

	/** Ask controller type */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * ConTypePanel;

	/** Playstation controls */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * PSPanel;

	/** Xbox controls */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * XBPanel;

	/** Show Playstation controls */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_PS;

	/** Show Xbox controls */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_XB;

public:
	/** Sets user widget interface variable */
	void SetWidgetInterface(IUserWidgetInterface * UserWidgetInt);

protected:
	/** Updates widget anytime it is constructed or edited */
	virtual void NativePreConstruct() override;

private:
	/** Shows mouse and keyboard inputs */
	UFUNCTION()
	void ViewMouseKeyInputs();

	/** Shows controller inputs */
	UFUNCTION()
	void ViewControllerInputs();

	/** Returns to previous menu layout */
	UFUNCTION()
	void ReturnToPrevious();

	/** Returns to previous menu layout on gamepad panel */
	void ReturnToGamepad();

	/** Returns to previous menu layout on main menu */
	void ReturnToMainMenu();

	/** Shows PS controls */
	UFUNCTION()
	void ShowPSControls();

	/** Shows XB controls */
	UFUNCTION()
	void ShowXBControls();

};
