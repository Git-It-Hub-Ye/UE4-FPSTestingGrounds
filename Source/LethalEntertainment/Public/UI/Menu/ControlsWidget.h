// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "UserWidgetInterface.h"
#include "ControlsWidget.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
class UMenuButtonsWidget;
class UScrollBox;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMannequinDelegate);

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

	/** How much to add to scroll offset based on input key */
	float AmountToAdd = 15.f;

	/** Max Length a scroll box can scroll */
	float MaxScrollLength = 0.f;


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
	UMenuButtonsWidget * Button_MouseKey;

	/** Allows user to return to previous panel */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Controller;

	/** Allows user to return to previous panel */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Back;


	////////////////////////////////////////////////////////////////////////////////
	// Mouse and Keyboard Panel

	/** Scroll to see all bindings */
	UPROPERTY(meta = (BindWidget))
	UScrollBox * ScrollBox_MouseKey;

	/** True when ScrollBox_MouseKey is in veiwport */
	bool bIsLookAtBindings_MouseKeys = false;

	/** Y axis length of ScrollBox_MouseKey, needed to get max scroll length */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 0.f))
	float SizeY_ScrollBox_MouseKey = 0.f;


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
	UMenuButtonsWidget * Button_PS;

	/** Show Xbox controls */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_XB;

	/** Scroll to see all bindings */
	UPROPERTY(meta = (BindWidget))
	UScrollBox * ScrollBox_PS;

	/** Scroll to see all bindings */
	UPROPERTY(meta = (BindWidget))
	UScrollBox * ScrollBox_XB;

	/** Stores name of last button clicked (Used to set button to focus on when returning to previous menu layouts) */
	FName Name_LastGamepadButton;

	/** True when ScrollBox_PS is in veiwport */
	bool bIsLookAtBindings_PS = false;

	/** True when ScrollBox_XB is in veiwport */
	bool bIsLookAtBindings_XB = false;

	/** Y axis length of ScrollBox_PS, needed to get max scroll length */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 0.f))
	float SizeY_ScrollBox_PS = 0.f;

	/** Y axis length of ScrollBox_XB, needed to get max scroll length */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 0.f))
	float SizeY_ScrollBox_XB = 0.f;

public:
	/** Sets user widget interface variable */
	void SetWidgetInterface(IUserWidgetInterface * UserWidgetInt);

	/** Sets the widget that should recieve focus when menu is first in viewport */
	void SetFocus();

	/** Use scroll box (if bShouldScrollUp - true = Upwards scroll & false = Downwards scroll) */
	void ScrollThroughMenu(bool bShouldScrollUp);

protected:
	virtual bool Initialize() override;

	virtual FReply NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent) override;

	virtual void NativeOnFocusLost(const FFocusEvent & InFocusEvent) override;

private:

	////////////////////////////////////////////////////////////////////////////////
	// Button Clicked

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
	// Button Hover

	/** Sets focus to Return button when hovered */
	UFUNCTION()
	void ButtonBackOnHover();

	/** Sets focus to MouseKey button when hovered */
	UFUNCTION()
	void ButtonMouseKeyOnHover();

	/** Sets focus to Controller button when hovered */
	UFUNCTION()
	void ButtonControllerOnHover();

	/** Sets focus to PS button when hovered */
	UFUNCTION()
	void ButtonPSOnHover();

	/** Sets focus to XB button when hovered */
	UFUNCTION()
	void ButtonXBOnHover();

	/** Resets all scroll menu bools to false */
	void ResetLookAtBindings();


	////////////////////////////////////////////////////////////////////////////////
	// Scroll Inputs

	/** Scrolls through menus in given direction */
	void MoveScrollBox(UScrollBox * ScrollBox, bool bShouldScrollUp);

	/** True when scroll box is in veiwport */
	bool GetIsScrollableMenu();

	/** Sets the max scroll of current scroll box in viewport */
	void SetMaxScrollLength(UScrollBox * ScrollBox, float SizeY_ScrollBox);

};
