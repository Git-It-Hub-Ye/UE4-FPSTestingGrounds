// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "IconsWidget.h"

void UIconsWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetupBase();
	SetupIcon();
	SetupText();
	SetupOutline1();
	SetupOutline2();
	SetupGlass();
}

void UIconsWidget::SetupBase()
{
	if (bDisplayBase == true)
	{
		if (EBase == EBaseShape::ESquare && Base_Square_Tex != NULL)
		{
			Base_Image->SetBrushFromTexture(Base_Square_Tex, true);
		}
		else if (EBase == EBaseShape::ECircle && Base_Circle_Tex != NULL)
		{
			Base_Image->SetBrushFromTexture(Base_Circle_Tex, true);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("No Base_Image in Icons Widget")) }

		Base_Image->SetColorAndOpacity(Base_Colour);
	}
	else
	{
		Base_Image->SetOpacity(0.f);
	}

	SetupBaseMask();
}

void UIconsWidget::SetupBaseMask()
{
	if (bDisplayBaseMask == true)
	{
		if (EBase == EBaseShape::ESquare && Square_Mask_Tex != NULL)
		{
			Base_Mask->SetBrushFromTexture(Square_Mask_Tex, true);
		}
		else if (EBase == EBaseShape::ECircle && Circle_Mask_Tex != NULL)
		{
			Base_Mask->SetBrushFromTexture(Circle_Mask_Tex, true);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("No Base_Mask in Icons Widget")) }

		Base_Mask->SetColorAndOpacity(Base_MaskColour);
	}
	else
	{
		Base_Mask->SetOpacity(0.f);
	}
}

void UIconsWidget::SetupIcon()
{
	if (bDisplayIcon == true)
	{
		if (Icon_Tex != NULL)
		{
			Icon_Image->SetBrushFromTexture(Icon_Tex, true);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("No Icon_Image in Icons Widget")) }

		Icon_Image->SetColorAndOpacity(Icon_Colour);
		Icon_Image->SetRenderTranslation(FVector2D(Icon_TranslationX, Icon_TranslationY));
		Icon_Image->SetRenderScale(FVector2D(Icon_Scale, Icon_Scale));
	}
	else
	{
		Icon_Image->SetOpacity(0.f);
	}
}

void UIconsWidget::SetupText()
{
	if (bDisplayText == true)
	{
		if (!Text_Block->IsValidLowLevel()) { UE_LOG(LogTemp, Warning, TEXT("Text_Block invalid in Icons Widget")) return; }
	
		Text_Block->SetText(Text_Edit);
		Text_Block->SetColorAndOpacity(Text_Colour);
		Text_Block->SetRenderTransform(Text_Transform);
		Text_Block->SetFont(Text_Font);
	}
	else
	{
		Text_Block->SetOpacity(0.f);
	}
}

void UIconsWidget::SetupOutline1()
{
	if (bDisplayOutline_1 == true)
	{
		if (EBase == EBaseShape::ESquare)
		{
			if (Outline_Square_1 != NULL)
			{
				Outline_1_Image->SetBrushFromTexture(Outline_Square_1, true);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("No Outline_Square_1 in Icons Widget")) }
		}
		else if (EBase == EBaseShape::ECircle)
		{
			if (Outline_Circle_1 != NULL)
			{
				Outline_1_Image->SetBrushFromTexture(Outline_Circle_1, true);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("No Outline_Circle_1 in Icons Widget")) }
		}

		Outline_1_Image->SetColorAndOpacity(Outline_1_Colour);
	}
	else
	{
		Outline_1_Image->SetOpacity(0.f);
	}
}

void UIconsWidget::SetupOutline2()
{
	if (bDisplayOutline_2 == true)
	{
		if (EBase == EBaseShape::ESquare)
		{
			if (Outline_Square_2 != NULL)
			{
				Outline_2_Image->SetBrushFromTexture(Outline_Square_2, true);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("No Outline_Square_2 in Icons Widget")) }
		}
		else if (EBase == EBaseShape::ECircle)
		{
			if (Outline_Circle_2 != NULL)
			{
				Outline_2_Image->SetBrushFromTexture(Outline_Circle_2, true);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("No Outline_Circle_2 in Icons Widget")) }
		}

		Outline_2_Image->SetColorAndOpacity(Outline_2_Colour);
	}
	else
	{
		Outline_2_Image->SetOpacity(0.f);
	}
}

void UIconsWidget::SetupGlass()
{
	if (bDisplayGlass == true)
	{
		if (EBase == EBaseShape::ESquare)
		{
			if (EGlass == EGlassType::EGlassType1 && Glass_Square_1 != NULL)
			{
				Glass_Image->SetBrushFromTexture(Glass_Square_1, true);
			}
			else if (EGlass == EGlassType::EGlassType2 && Glass_Square_2 != NULL)
			{
				Glass_Image->SetBrushFromTexture(Glass_Square_2, true);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("No Square Glass_Image in Icons Widget")) }
		}
		else if (EBase == EBaseShape::ECircle)
		{
			if (EGlass == EGlassType::EGlassType1 && Glass_Circle_1 != NULL)
			{
				Glass_Image->SetBrushFromTexture(Glass_Circle_1, true);
			}
			else if (EGlass == EGlassType::EGlassType2 && Glass_Circle_2 != NULL)
			{
				Glass_Image->SetBrushFromTexture(Glass_Circle_2, true);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("No Circle Glass_Image in Icons Widget")) }
		}

		Glass_Image->SetColorAndOpacity(Glass_Colour);
	}
	else
	{
		Glass_Image->SetOpacity(0.f);
	}
}

