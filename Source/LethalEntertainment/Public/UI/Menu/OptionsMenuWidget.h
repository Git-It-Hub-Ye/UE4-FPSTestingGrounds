// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidget.h"
#include "OptionsMenuWidget.generated.h"

class UMenuButtonsWidget;
class USliderWidget;

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

	/** Allows user to apply changes to options */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Apply;

	/** Allows user to reset options back to default */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Reset;

	/** Allows user to go back to previous menu */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Back;


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

public:
	////////////////////////////////////////////////////////////////////////////////
	// Setup

	/** Sets user widget interface variable */
	void SetUserWidgetInterface(IUserWidgetInterface * UserWidgetInt);

	/** Set options values */
	void SetInitialValues();

	/** Sets the widget that should recieve focus when menu is first in viewport */
	void SetFocus();

protected:
	/** Setup widget input functions */
	virtual bool Initialize() override;

	/** Updates widget anytime it is constructed or edited */
	virtual void NativePreConstruct() override;

	/** Any behavior after Super is carried out after construction */
	virtual void NativeConstruct() override;

private:
	////////////////////////////////////////////////////////////////////////////////
	// Setup

	/** Sets values to display */
	void SetUserSettingsValue(float MouseSens, float ADS_MouseSens, float ConSens, float ADS_ConSens);


	////////////////////////////////////////////////////////////////////////////////
	// Button Clicked

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
