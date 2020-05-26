// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Fonts/SlateFontInfo.h"
#include "SliderWidget.generated.h"

class UBorder;
class USlider;
class UTextBlock;
class UProgressBar;

/** On Focused Event */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFocusSliderDelegate, UWidget*, Widget);

/**
 * Slider widget for menus, adjust to change value
 */
UCLASS()
class LETHALENTERTAINMENT_API USliderWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	////////////////////////////////////////////////////////////////////////////////
	// Slider

	/** Background */
	UPROPERTY(meta = (BindWidget))
	UBorder * BackgroundPanel;

	/** Slider */
	UPROPERTY(meta = (BindWidget))
	USlider * Slider;

	/** Progress bar for visualising slider */
	UPROPERTY(meta = (BindWidget))
	UProgressBar * ProgressBar;

	/** Current value of slider */
	float Value_Current;

	/** Default Colour of background */
	UPROPERTY(EditAnywhere, DisplayName = "Default Background Colour", Category = "Config")
	FLinearColor Colour_Default;

	/** Focused Colour of background */
	UPROPERTY(EditAnywhere, DisplayName = "Focused Background Colour", Category = "Config")
	FLinearColor Colour_Focus;

	
	////////////////////////////////////////////////////////////////////////////////
	// Text

	/** Displays Value */
	UPROPERTY(meta = (BindWidget))
	UTextBlock * Text_Value;

	/** Displays option text */
	UPROPERTY(meta = (BindWidget))
	UTextBlock * Text_Option;

	/** Text displayed to describe option */
	UPROPERTY(EditAnywhere, DisplayName = "Text Option", Category = "Config")
	FText Text_Edit;

public:

	////////////////////////////////////////////////////////////////////////////////
	// Setup

	/** Sets value of slider to current player sensitivity */
	void SetInitialValue(float Value);


	////////////////////////////////////////////////////////////////////////////////
	// Focus

	/** Broadcasts to Menus */
	FFocusSliderDelegate OnWidgetFocused;


	////////////////////////////////////////////////////////////////////////////////
	// Data

	/** Return Current Value */
	float GetCurrentValue() { return FMath::RoundToInt(Value_Current * 100); }

protected:
	////////////////////////////////////////////////////////////////////////////////
	// Setup

	/** Setup widget input functions */
	virtual bool Initialize() override;

	/** Updates widget anytime it is constructed or edited */
	virtual void NativePreConstruct() override;


	////////////////////////////////////////////////////////////////////////////////
	// Focus

	/** Activates when widget has focus */
	virtual void NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent) override;

	/** Activates when widget loses focus */
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent & InFocusEvent) override;


	////////////////////////////////////////////////////////////////////////////////
	// Scroll Inputs

	/** Listens for down inputs */
	virtual FReply NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent) override;

private:

	////////////////////////////////////////////////////////////////////////////////
	// Focus

	/** Sets background colour */
	void SetBackgroundColour(FLinearColor Colour);


	////////////////////////////////////////////////////////////////////////////////
	// Scroll Inputs

	/** Sets value of slider */
	void SetNewValue(float Value);

	/** Sets display text */
	void SetTextHeading();

	/** Sets display text */
	void SetTextValue();

	/** Updates ProgressBar fill */
	void UpdateProgressBar(float Value);

	/** Updates Text_Value and ProgressBar */
	UFUNCTION()
	void UpdateValue(float Value);

	/** Adds a value to slider */
	void AdjustSlider(bool IncreaseSliderValue);

};
