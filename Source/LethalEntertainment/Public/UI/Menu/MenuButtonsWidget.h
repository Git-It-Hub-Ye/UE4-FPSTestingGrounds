// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Fonts/SlateFontInfo.h"
#include "MenuButtonsWidget.generated.h"

class UButton;
class UTextBlock;

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

	/** Button Colour */
	UPROPERTY(EditAnywhere, DisplayName = "Button Colour", Category = "Button")
	FLinearColor Button_Colour = FColor::Silver;


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
	void SetFocusToButton();

	UTextBlock * GetTextToCustomise() { return Button_Text; }

	UButton * GetButton();

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
