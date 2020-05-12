// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateTypes.h"
#include "Fonts/SlateFontInfo.h"
#include "MenuButtonsWidget.generated.h"

class UButton;
class UTextBlock;

/** On Focused Event */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFocusButtonDelegate, UWidget*, Widget);

/**
 * Button for menus (Sets Hover Image, Sound etc when focus is recieved)
 */
UCLASS()
class LETHALENTERTAINMENT_API UMenuButtonsWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	////////////////////////////////////////////////////////////////////////////////
	// Button

	/** Button */
	UPROPERTY(meta = (BindWidget))
	UButton * Button_Main;

	/** Buttons default colour */
	UPROPERTY(EditAnywhere, DisplayName = "Default Colour", Category = "Button")
	FSlateBrush Colour_Default;

	/** Button on hover colour */
	UPROPERTY(EditAnywhere, DisplayName = "Hover Colour", Category = "Button")
	FSlateBrush Colour_Hover;

	/** Button on hover colour */
	UPROPERTY(EditAnywhere, DisplayName = "Pressed Colour", Category = "Button")
	FSlateBrush Colour_Pressed;

	/** Used to set style */
	FButtonStyle Style_Default;

	/** Used to set style */
	FButtonStyle Style_Focused;


	////////////////////////////////////////////////////////////////////////////////
	// Text

	/** Text block which will be on button */
	UPROPERTY(meta = (BindWidget))
	UTextBlock * Button_Text;

	/** Text to be displayed */
	UPROPERTY(EditAnywhere, DisplayName = "Text", Category = "Text")
	FText Text_Edit = FText::FromName("Text");

	/** Text font */
	UPROPERTY(EditAnywhere, DisplayName = "Text Font", Category = "Text")
	FSlateFontInfo Text_Font;

	/** Sets location of text */
	UPROPERTY(EditAnywhere, DisplayName = "Text Transform", Category = "Text")
	FWidgetTransform Text_Transform;

	/** Text Colour */
	UPROPERTY(EditAnywhere, DisplayName = "Colour", Category = "Text")
	FLinearColor Text_Colour = FColor::White;

public:
	/** Sets focus to Button */
	void SetFocusToButton();

	/** Returns text block */
	UTextBlock * GetTextToCustomise() { return Button_Text; }

	/** Returns button widget */
	UButton * GetButton();

	/** Broadcasts to Menus */
	FFocusButtonDelegate OnWidgetFocused;

protected:
	/** Updates widget anytime it is constructed or edited */
	virtual void NativePreConstruct() override;

	/** Activates when widget has focus */
	virtual void NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent) override;

	/** Activates when widget loses focus */
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent & InFocusEvent) override;

private:
	/** Setup button */
	void SetupButton();

	/** Displays text */
	void SetupText();

	/** Activates hover style of button */
	void SetOnHoverStyle();

	/** Sets button style back to default */
	void SetUnhoverStyle();
	
};
