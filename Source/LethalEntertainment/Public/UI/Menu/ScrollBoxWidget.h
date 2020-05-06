// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetInterface.h"
#include "ScrollBoxWidget.generated.h"

class UScrollBox;

/**
* Scroll Box for menus (Sets Bar Hover when focus is recieved)
*/
UCLASS()
class LETHALENTERTAINMENT_API UScrollBoxWidget : public UUserWidget
{
	GENERATED_BODY()


private:
	/** Calls interface functions (These functions should be overriden by parent widget) */
	IUserWidgetInterface * UserWidgetInterface;

	/** Scroll Box */
	UPROPERTY(meta = (BindWidget))
	UScrollBox * ScrollBox_Main;

	/** Scroll bars default colour */
	UPROPERTY(EditAnywhere, DisplayName = "Default Scroll Bar Colour", Category = "Scroll Box")
	FSlateBrush Colour_Default;

	/** Scroll bars colour when focused on by player*/
	UPROPERTY(EditAnywhere, DisplayName = "Focused Scroll Bar Colour", Category = "Scroll Box")
	FSlateBrush Colour_Focus;

	/** Max offset of scroll box */
	float MaxScrollOffset = 0.f;

	float ScrollBoxSizeY = 0.f;

	/** How much to add to scroll offset based on input key */
	float AmountToAdd = 15.f;

public:
	////////////////////////////////////////////////////////////////////////////////
	// Setup

	void SetUserWidgetInterface(IUserWidgetInterface * UserInterface);

	void SetSizeYOfScrollBox(float SizeY);

	/** Reset croll offset back to zero */
	void ResetOffset();

protected:
	////////////////////////////////////////////////////////////////////////////////
	// Setup

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

	virtual FReply NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent) override;

private:
	////////////////////////////////////////////////////////////////////////////////
	// Setup

	/** Setup scroll bar colours */
	void SetupScrollBar();


	////////////////////////////////////////////////////////////////////////////////
	// Focus

	/** Activates focused style of scroll bar */
	void SetFocusStyle();

	/** Sets scroll bar style back to default */
	void SetUnfocusStyle();

	/** Navigates up from scroll box, calls parent through IUserWidgetInterface  */
	void OnNavUp();

	/** Navigates down from scroll box, calls parent through IUserWidgetInterface  */
	void OnNavDown();


	////////////////////////////////////////////////////////////////////////////////
	// Scroll Inputs

	/** Use scroll box (if bShouldScrollUp: true = Upwards scroll & false = Downwards scroll) */
	void ScrollThroughMenu();

	/** Max offset of scroll bar */
	void GetMaxScrollOffset(float SizeY);
	
};
