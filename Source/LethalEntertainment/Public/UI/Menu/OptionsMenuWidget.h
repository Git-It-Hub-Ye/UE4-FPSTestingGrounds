// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "OptionsMenuWidget.generated.h"

class UMenuButtonsWidget;
class USliderWidget;
class UIconsWidget;

/**
 * Allows user to view and change settings
 */
UCLASS()
class LETHALENTERTAINMENT_API UOptionsMenuWidget : public UMenuWidget
{
	GENERATED_BODY()
	
private:
	/** Calls interface functions (These functions should be overriden by parent widget) */
	IUserWidgetInterface * UserWidgetInterface;


	////////////////////////////////////////////////////////////////////////////////
	// Options Panel

	/** Adjustable slider for mouse sensitivity */
	UPROPERTY(meta = (BindWidget))
	USliderWidget * Slider_MouseSens;

	/** Adjustable slider for controller sensitivity */
	UPROPERTY(meta = (BindWidget))
	USliderWidget * Slider_ADS_MouseSens;

	/** Adjustable slider for controller sensitivity */
	UPROPERTY(meta = (BindWidget))
	USliderWidget * Slider_ConSens;

	/** Adjustable slider for controller sensitivity */
	UPROPERTY(meta = (BindWidget))
	USliderWidget * Slider_ADS_ConSens;

	/** Allows user to turn off/on invert for y axis */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_InvertY;

	/** Allows user to turn off/on vibration for controller */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Vibration;

	/** Allows user to apply changes to options */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Apply;

	/** Allows user to reset options back to default */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Reset;

	/** Allows user to go back to previous menu */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Back;

	/** Switches between different menu icons depending on control type */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher * WidgetSwitcherIcons;

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
	// Default Values

	/** Stores default value for mouse sensitivity */
	float Default_MouseSens = 0.f;

	/** Stores default value for mouse ADS sensitivity */
	float Default_ADS_MouseSens = 0.f;

	/** Stores default value for controller sensitivity */
	float Default_ConSens = 0.f;

	/** Stores default value for controller ADS sensitivity */
	float Default_ADS_ConSens = 0.f;

	/** Stores default value for Invert Y Axis */
	bool Default_InvertY = false;

	/** Stores default value for controller vibration */
	bool Default_Vibration = true;


	////////////////////////////////////////////////////////////////////////////////
	// Current Values

	/** Stores current value for mouse sensitivity */
	float Current_MouseSens = 0.f;

	/** Stores current value for mouse ADS sensitivity */
	float Current_ADS_MouseSens = 0.f;

	/** Stores current value for controller sensitivity */
	float Current_ConSens = 0.f;

	/** Stores current value for controller ADS sensitivity */
	float Current_ADS_ConSens = 0.f;

	/** Stores current value for Invert Y Axis */
	bool Current_InvertY = false;

	/** Stores current value for vibration */
	bool Current_Vibration = false;

public:
	////////////////////////////////////////////////////////////////////////////////
	// Setup

	/** Sets user widget interface variable */
	void SetUserWidgetInterface(IUserWidgetInterface * UserWidgetInt);

	/** Set options values */
	void SetInitialValues();

	/** Sets the widget that should recieve focus when menu is first in viewport */
	void SetFocus(EControlType IconControlType);

protected:
	/** Setup widget input functions */
	virtual bool Initialize() override;

	/** Updates widget anytime it is constructed or edited */
	virtual void NativePreConstruct() override;

private:
	////////////////////////////////////////////////////////////////////////////////
	// Setup

	/** Sets values to display */
	void SetUserSettingsValue(float MouseSens, float ADS_MouseSens, float ConSens, float ADS_ConSens, bool Invert_Y, bool Vibration);


	////////////////////////////////////////////////////////////////////////////////
	// Display

	void SetControlsIcon(UIconsWidget * Icon);


	////////////////////////////////////////////////////////////////////////////////
	// Button Clicked

	/** Toggles invert Y axis off/on */
	UFUNCTION()
	void ToggleInvertY();

	/** Toggles controller vibration off/on */
	UFUNCTION()
	void ToggleVibration();

	/** Applys new values to user settings */
	UFUNCTION()
	void ApplyChanges();

	/** Resets values back to default */
	UFUNCTION()
	void ResetToDefaults();

	/** Returns to previous menu layout */
	UFUNCTION()
	void ReturnToPrevious();


	////////////////////////////////////////////////////////////////////////////////
	// Button Hover

	/** Sets focus to InvertY button when hovered */
	UFUNCTION()
	void ButtonInvertYOnHover();

	/** Sets focus to Vibration button when hovered */
	UFUNCTION()
	void ButtonVibrationOnHover();

	/** Sets focus to Apply button when hovered */
	UFUNCTION()
	void ButtonApplyOnHover();

	/** Sets focus to Reset button when hovered */
	UFUNCTION()
	void ButtonResetOnHover();

	/** Sets focus to Return button when hovered */
	UFUNCTION()
	void ButtonBackOnHover();


	////////////////////////////////////////////////////////////////////////////////
	// Inputs

	/** Returns back to previous menu */
	virtual void EscInput() override { BackInput(); }

	/** Returns back to previous menu */
	virtual void BackInput() override;

	/** Instantly closes menu */
	virtual void CloseMenuInput() override;
	
};
