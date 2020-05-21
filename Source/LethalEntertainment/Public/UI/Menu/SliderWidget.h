// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Fonts/SlateFontInfo.h"
#include "SliderWidget.generated.h"

class USlider;
class UTextBlock;
class UProgressBar;

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

	/** Slider */
	UPROPERTY(meta = (BindWidget))
	USlider * Slider;

	/** Progress bar for visualising slider */
	UPROPERTY(meta = (BindWidget))
	UProgressBar * ProgressBar;

	/** Current value of slider */
	float Value_Current;


	////////////////////////////////////////////////////////////////////////////////
	// Text

	/** Displays Value */
	UPROPERTY(meta = (BindWidget))
	UTextBlock * Text_Value;

	/** Displays option heading */
	UPROPERTY(meta = (BindWidget))
	UTextBlock * Text_Option;

	/** Text displayed to describe option */
	UPROPERTY(EditAnywhere, DisplayName = "Text Option", Category = "Config")
	FText Text_Edit;

public:
	/** Sets value of slider to current player sensitivity */
	void SetInitialValue(float Value);

protected:
	/** Setup widget input functions */
	virtual bool Initialize() override;

	/** Updates widget anytime it is constructed or edited */
	virtual void NativePreConstruct() override;

	/** Any behavior after Super is carried out after construction */
	virtual void NativeConstruct() override;

private:
	/** Sets value of slider */
	void SetNewValue(float Value);

	/** Sets display text */
	void SetTextHeading();

	/** Sets display text */
	void SetTextValue();

	/** Updates Text_Value and ProgressBar */
	UFUNCTION()
	void UpdateValue(float Value);

	/** Updates ProgressBar fill */
	void UpdateProgressBar(float Value);

};
