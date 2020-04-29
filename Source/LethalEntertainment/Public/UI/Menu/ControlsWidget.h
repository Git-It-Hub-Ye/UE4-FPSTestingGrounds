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

	/** Allows user to return to previous panel */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_ControlType;

	/** Allows user to return to previous panel */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonsWidget * Button_Back;


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
	// Widget Switcher Controls

	/** Scroll to see all bindings */
	UPROPERTY(meta = (BindWidget))
	UScrollBox * ScrollBox_MouseKey;

	/** Scroll to see all bindings */
	UPROPERTY(meta = (BindWidget))
	UScrollBox * ScrollBox_PS;

	/** Scroll to see all bindings */
	UPROPERTY(meta = (BindWidget))
	UScrollBox * ScrollBox_XB;

	/** Contains data for scroll box input */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Scroll box data", Category = "Config")
	FScrollBoxData ScrollBoxData;

	/** True when ScrollBox_MouseKey is in veiwport */
	bool bIsLookAtBindings_MouseKeys = false;

	/** True when ScrollBox_PS is in veiwport */
	bool bIsLookAtBindings_PS = false;

	/** True when ScrollBox_XB is in veiwport */
	bool bIsLookAtBindings_XB = false;

public:
	/** Sets user widget interface variable */
	void SetWidgetInterface(IUserWidgetInterface * UserWidgetInt);

	/** Sets the widget that should recieve focus when menu is first in viewport */
	void SetFocus();

	/** Use scroll box (if bShouldScrollUp: true = Upwards scroll & false = Downwards scroll) */
	void ScrollThroughMenu(bool bShouldScrollUp);

protected:
	virtual bool Initialize() override;

	virtual FReply NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent) override;

	virtual void NativeConstruct() override;

private:

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


	////////////////////////////////////////////////////////////////////////////////
	// Navigation Functions

	/** Called from a Blueprint Widget's child widgets navigation (Used for custom navigation) */
	UFUNCTION(BlueprintCallable)
	void OnNavigatedToScrollMenu();

	/** Called from a Blueprint Widget's child widgets navigation (Used for custom navigation) */
	UFUNCTION(BlueprintCallable)
	void OnNavToConTypeButton();

	/** Called from a Blueprint Widget's child widgets navigation (Used for custom navigation) */
	UFUNCTION(BlueprintCallable)
	void OnNavToBackButton();

};
