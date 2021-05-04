// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "UserWidgetInterface.h"
#include "ControlsWidget.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
class UMenuButtonsWidget;
class UScrollBoxWidget;
class UIconsWidget;

/** Size data for scroll box */
USTRUCT(BlueprintType)
struct FScrollBoxData {
	GENERATED_USTRUCT_BODY()

	/** Y axis length of ScrollBox_MouseKey, needed to get max scroll length */
	UPROPERTY(EditDefaultsOnly, DisplayName = "MouseKey Scroll box Size Y", Category = "Config", meta = (ClampMin = 0.f))
	float SizeY_ScrollBox_MouseKey;

	/** Y axis length of ScrollBox_PS, needed to get max scroll length */
	UPROPERTY(EditDefaultsOnly, DisplayName = "PS Scroll box Size Y", Category = "Config", meta = (ClampMin = 0.f))
	float SizeY_ScrollBox_PS;

	/** Y axis length of ScrollBox_XB, needed to get max scroll length */
	UPROPERTY(EditDefaultsOnly, DisplayName = "XB Scroll box Size Y", Category = "Config", meta = (ClampMin = 0.f))
	float SizeY_ScrollBox_XB;

	FScrollBoxData()
	{
		SizeY_ScrollBox_MouseKey = 0.f;
		SizeY_ScrollBox_PS = 0.f;
		SizeY_ScrollBox_XB = 0.f;
	}
};

/** On Control Type Changed Event */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FControlTypeDelegate, EControlType, NewControlType);

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

	/** Switches between different menu icons depending on control type */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher * WidgetSwitcherIcons;

	/** Allows user to return to previous panel */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_ControlType;

	/** Allows user to return to previous panel */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Back;

	/** Back icon for keyboard */
	UPROPERTY(meta = (BindWidget))
	UIconsWidget * Icon_Key;

	/** Back icon for PS controller */
	UPROPERTY(meta = (BindWidget))
	UIconsWidget * Icon_PS;

	/** Back icon for XB Controller */
	UPROPERTY(meta = (BindWidget))
	UIconsWidget * Icon_XB;


	////////////////////////////////////////////////////////////////////////////////
	// Control Type Panel

	/** Ask controller type */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget * ConTypePanel;

	/** Allows user to return to previous panel */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_MouseKey;

	/** Show Playstation controls */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_PS;

	/** Show Xbox controls */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_XB;

	/** Stores name of last button clicked (Used to set button to focus on when returning to previous menu layouts) */
	FName Name_CurrentControlType;


	////////////////////////////////////////////////////////////////////////////////
	// Widget Switcher

	/** Scroll to see all bindings */
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget * ScrollBox_MouseKey;

	/** Scroll to see all bindings */
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget * ScrollBox_PS;

	/** Scroll to see all bindings */
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget * ScrollBox_XB;

	/** Contains data for scroll box input */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Scroll box data", Category = "Config")
	FScrollBoxData ScrollBoxData;

public:
	/** Sets user widget interface variable */
	void SetUserWidgetInterface(IUserWidgetInterface * UserWidgetInt);

	/** Broadcast when control type is changed */
	FControlTypeDelegate OnControlTypeSet;

	/** Sets the widget that should recieve focus when menu is first in viewport */
	void SetFocus();

	/** Sets Controls type in scroll box */
	void SetScrollBoxType(EControlType IconControlType);

protected:
	/** Setup widget input functions */
	virtual bool Initialize() override;

	/** Any behavior after Super is carried out after construction */
	virtual void NativeConstruct() override;


	////////////////////////////////////////////////////////////////////////////////
	// IUserWidgetInterface Functions (Called by child widgets)

	/** Switch focus to widget above */
	virtual void OnNavUpToParent() override;

	/** Switch focus to widget below */
	virtual void OnNavDownToParent() override;


private:

	////////////////////////////////////////////////////////////////////////////////
	// Setup

	void SetScrollBoxInterface();


	////////////////////////////////////////////////////////////////////////////////
	// Button Clicked

	/** Shows controller inputs */
	UFUNCTION()
	void SelectControlType();

	/** Shows mouse and keyboard inputs */
	UFUNCTION()
	void ShowMouseKeyInputs();

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
	void ButtonControlTypeOnHover();

	/** Sets focus to PS button when hovered */
	UFUNCTION()
	void ButtonPSOnHover();

	/** Sets focus to XB button when hovered */
	UFUNCTION()
	void ButtonXBOnHover();


	////////////////////////////////////////////////////////////////////////////////
	// Navigation Functions

	/** Called from a Blueprint Widget's child widgets navigation (Used for custom navigation) */
	UFUNCTION(BlueprintCallable)
	void OnNavigatedToScrollMenu();


	////////////////////////////////////////////////////////////////////////////////
	// Display

	/** Displays correct scroll box for control type */
	void SetScrollMenu(UScrollBoxWidget * ScrollBox);

	/** Displays correct icons for control type */
	void SetControlsIcon(UIconsWidget * Icon);


	////////////////////////////////////////////////////////////////////////////////
	// Focus

	UFUNCTION()
	void SetScrollBoxFocusedWidget(UWidget * Widget);

	/** Reassign focus to a widget */
	virtual void ReassignFocus() override;


	////////////////////////////////////////////////////////////////////////////////
	// Inputs

	/** Returns back to previous menu */
	virtual void EscInput() override { BackInput(); }

	/** Returns back to previous menu */
	virtual void BackInput() override;

	/** Instantly closes menu */
	virtual void CloseMenuInput() override;


	////////////////////////////////////////////////////////////////////////////////
	// Menu Data

	/** Get current scroll box displayed */
	UScrollBoxWidget * GetCurrentScrollBox();

};
