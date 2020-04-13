// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "UserWidgetInterface.h"
#include "ControlsWidget.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
class UButton;
class UScrollBox;

/**
 * Displays mouse/key bindings and gamepad controls
 */
UCLASS()
class LETHALENTERTAINMENT_API UControlsWidget : public UMenuWidget
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
	// Mouse and Keyboard Panel

	/** Allows user to return to previous panel */
	UPROPERTY(meta = (BindWidget))
	UScrollBox * Scroll_MouseKey;


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

	/** Stores name of last button clicked (Used to set button to focus on when returning to previous menu layouts) */
	FName Name_LastGamepadButton;

public:
	/** Sets user widget interface variable */
	void SetWidgetInterface(IUserWidgetInterface * UserWidgetInt);

	void SetFocus();

protected:
	virtual bool Initialize() override;

	virtual FReply NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent) override;

private:

	////////////////////////////////////////////////////////////////////////////////
	// On clicked

	/** Shows mouse and keyboard inputs */
	UFUNCTION()
	void ViewMouseKeyInputs();

	/** Shows controller inputs */
	UFUNCTION()
	void ViewControllerInputs();

	/** Shows PS controls */
	UFUNCTION()
	void ShowPSControls();

	/** Shows XB controls */
	UFUNCTION()
	void ShowXBControls();

	/** Returns to previous menu layout */
	UFUNCTION()
	void ReturnToPrevious();


	////////////////////////////////////////////////////////////////////////////////
	// Inputs

	void ScrollBoxDown();

};
