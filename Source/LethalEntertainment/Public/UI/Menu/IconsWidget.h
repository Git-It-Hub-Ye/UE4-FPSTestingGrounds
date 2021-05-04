// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Fonts/SlateFontInfo.h"
#include "IconsWidget.generated.h"

class UOverlay;
class UImage;
class UTexture2D;
class UTextBlock;

/** Determine base shape */
UENUM()
enum class EBaseShape : uint8 {
	ESquare		UMETA(DisplayName = "Square"),
	ECircle		UMETA(DisplayName = "Circle")
};

UENUM()
enum class EGlassType :uint8 {
	EGlassType1 UMETA(DisplayName = "Glass 1"),
	EGlassType2 UMETA(DisplayName = "Glass 2")
};

/**
 * Displays icons (Such as Board/Mouse or Gamepad controls)
 */
UCLASS()
class LETHALENTERTAINMENT_API UIconsWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	////////////////////////////////////////////////////////////////////////////////
	// Panel

	/** Allows widgets to be stacked on top of each other */
	UPROPERTY(meta = (BindWidget))
	UOverlay * Overlay;


	////////////////////////////////////////////////////////////////////////////////
	// Base

	/** Base image */
	UPROPERTY(meta = (BindWidget))
	UImage * Base_Image;

	/** Texture to be displayed as ENUM Square base */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Square Base", Category = "Base")
	UTexture2D * Base_Square_Tex;

	/** Texture to be displayed as ENUM Circle base */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Circle Base", Category = "Base")
	UTexture2D * Base_Circle_Tex;

	/** Texture to be displayed as Square mask */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Square Mask", Category = "Base")
	UTexture2D * Square_Mask_Tex;

	/** Texture to be displayed as Circle mask */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Circle Mask", Category = "Base")
	UTexture2D * Circle_Mask_Tex;

	/** Base is visible when true */
	UPROPERTY(EditAnywhere, DisplayName = "Display Base", Category = "Base")
	bool bDisplayBase;

	/** Select base shape */
	UPROPERTY(EditAnywhere, DisplayName = "Base", Category = "Base")
	EBaseShape EBase = EBaseShape::ESquare;

	/** Base Colour */
	UPROPERTY(EditAnywhere, DisplayName = "Base Colour", Category = "Base")
	FLinearColor Base_Colour;

	/** Displays Base Mask */
	UPROPERTY(meta = (BindWidget))
	UImage * Base_Mask;

	/** Base mask is visible when true */
	UPROPERTY(EditAnywhere, DisplayName = "Display Base Mask", Category = "Base")
	bool bDisplayBaseMask;

	/** Mask Colour */
	UPROPERTY(EditAnywhere, DisplayName = "Base Mask Colour", Category = "Base")
	FLinearColor Base_MaskColour;


	////////////////////////////////////////////////////////////////////////////////
	// Icon

	/** Icon image */
	UPROPERTY(meta = (BindWidget))
	UImage * Icon_Image;

	/** Icon is visible when true */
	UPROPERTY(EditAnywhere, DisplayName = "Display Icon", Category = "Icon")
	bool bDisplayIcon;

	/** Texture to be displayed as icon */
	UPROPERTY(EditAnywhere, DisplayName = "Icon", Category = "Icon")
	UTexture2D * Icon_Tex;

	/** Icon scale */
	UPROPERTY(EditAnywhere, DisplayName = "Icon Scale", Category = "Icon", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float Icon_Scale;

	/** Icon position along X axis */
	UPROPERTY(EditAnywhere, DisplayName = "Icon Translation X", Category = "Icon")
	float Icon_TranslationX;

	/** Icon position along Y axis */
	UPROPERTY(EditAnywhere, DisplayName = "Icon Translation Y", Category = "Icon")
	float Icon_TranslationY;

	/** Icon Colour */
	UPROPERTY(EditAnywhere, DisplayName = "Icon Colour", Category = "Icon")
	FLinearColor Icon_Colour;


	////////////////////////////////////////////////////////////////////////////////
	// Text

	/** Text */
	UPROPERTY(meta = (BindWidget))
	UTextBlock * Text_Block;

	/** Text is visible when true */
	UPROPERTY(EditAnywhere, DisplayName = "Display Text", Category = "Text")
	bool bDisplayText;

	/** Text to be displayed */
	UPROPERTY(EditAnywhere, DisplayName = "Text", Category = "Text")
	FText Text_Edit;
	
	/** Text font */
	UPROPERTY(EditAnywhere, DisplayName = "Text Font", Category = "Text")
	FSlateFontInfo Text_Font;

	/** Sets location of text */
	UPROPERTY(EditAnywhere, DisplayName = "Text Transform", Category = "Text")
	FWidgetTransform Text_Transform;

	/** Text Colour */
	UPROPERTY(EditAnywhere, DisplayName = "Colour", Category = "Text")
	FLinearColor Text_Colour;


	////////////////////////////////////////////////////////////////////////////////
	// Outline

	/** Outline 1 image */
	UPROPERTY(meta = (BindWidget))
	UImage * Outline_1_Image;

	/** Texture to be displayed as outline 1 for square */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Outline Square 1", Category = "Outline")
	UTexture2D * Outline_Square_1;

	/** Texture to be displayed as outline 2 for square */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Outline Square 2", Category = "Outline")
	UTexture2D * Outline_Square_2;

	/** Texture to be displayed as outline 1 for circle */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Outline Circle 1", Category = "Outline")
	UTexture2D * Outline_Circle_1;

	/** Texture to be displayed as outline 2 for circle */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Outline Circle 2", Category = "Outline")
	UTexture2D * Outline_Circle_2;

	/** Outline is visible when true */
	UPROPERTY(EditAnywhere, DisplayName = "Display Outline 1", Category = "Outline")
	bool bDisplayOutline_1;

	/** Outline Colour */
	UPROPERTY(EditAnywhere, DisplayName = "Outline 1 Colour", Category = "Outline")
	FLinearColor Outline_1_Colour;

	/** Ooutline 2 image */
	UPROPERTY(meta = (BindWidget))
	UImage * Outline_2_Image;

	/** Outline is visible when true */
	UPROPERTY(EditAnywhere, DisplayName = "Display Outline 2", Category = "Outline")
	bool bDisplayOutline_2;

	/** Outline Colour */
	UPROPERTY(EditAnywhere, DisplayName = "Outline 2 Colour", Category = "Outline")
	FLinearColor Outline_2_Colour;


	////////////////////////////////////////////////////////////////////////////////
	// Glass

	/** Glass image */
	UPROPERTY(meta = (BindWidget))
	UImage * Glass_Image;

	/** Texture to be displayed as ENUM Glass 1 for square */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Glass Square 1", Category = "Glass")
	UTexture2D * Glass_Square_1;

	/** Texture to be displayed as ENUM Glass 2 for square */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Glass Square 2", Category = "Glass")
	UTexture2D * Glass_Square_2;

	/** Texture to be displayed as ENUM Glass 1 for circle */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Glass Circle 1", Category = "Glass")
	UTexture2D * Glass_Circle_1;

	/** Texture to be displayed as ENUM Glass 2 for circle */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Glass Circle 2", Category = "Glass")
	UTexture2D * Glass_Circle_2;

	/** Glass is visible when true */
	UPROPERTY(EditAnywhere, DisplayName = "Display Glass", Category = "Glass")
	bool bDisplayGlass;

	/** Select glass type */
	UPROPERTY(EditAnywhere, DisplayName = "Glass", Category = "Glass")
	EGlassType EGlass = EGlassType::EGlassType1;

	/** Glass Colour */
	UPROPERTY(EditAnywhere, DisplayName = "Colour", Category = "Glass")
	FLinearColor Glass_Colour;


protected:
	/** Updates widget anytime it is constructed or edited */
	virtual void NativePreConstruct() override;

private:
	/** Displays base */
	void SetupBase();

	/** Displays base mask */
	void SetupBaseMask();

	/** Displays icon */
	void SetupIcon();

	/** Displays text */
	void SetupText();

	/** Displays outline 1 */
	void SetupOutline1();

	/** Displays outline 2 */
	void SetupOutline2();

	/** Displays glass */
	void SetupGlass();

};
