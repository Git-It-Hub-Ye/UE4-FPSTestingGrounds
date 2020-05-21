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
	USliderWidget * Slider_ConSens;

	/** Allows user to go back to previous menu */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Back;

public:
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
	// Button Clicked

	/** Returns to previous menu layout */
	UFUNCTION()
	void ReturnToPrevious();


	////////////////////////////////////////////////////////////////////////////////
	// Button Hover

	/** Sets focus to Return button when hovered */
	UFUNCTION()
	void ButtonBackOnHover();
	
};
