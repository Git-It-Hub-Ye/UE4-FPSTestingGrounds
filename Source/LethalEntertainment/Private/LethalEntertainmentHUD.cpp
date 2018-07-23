// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "LethalEntertainment.h"
#include "LethalEntertainmentHUD.h"
#include "Player/Mannequin.h"
#include "Player/CharacterPlayerController.h"
#include "Weapons/Weapons.h"
#include "Weapons/Weapon_InstantFire.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"

ALethalEntertainmentHUD::ALethalEntertainmentHUD()
{
}

void ALethalEntertainmentHUD::DrawHUD()
{
	Super::DrawHUD();
	DrawCrosshair();
}

void ALethalEntertainmentHUD::DrawCrosshair()
{
	APlayerController * PCOwner = Cast<APlayerController>(PlayerOwner);
	if (PCOwner)
	{
		AMannequin * Pawn = Cast<AMannequin>(PCOwner->GetPawn());
		if (Pawn && Pawn->GetWeapon()) 
		{
			AWeapons * Weapon = Pawn->GetWeapon();
			AWeapon_InstantFire * InstWeapon = Cast<AWeapon_InstantFire>(Pawn->GetWeapon());

			if (!Pawn->GetIsBlocked() && Weapon)
			{
				if (!Pawn->IsAiming() && Weapon->ShowCrosshair())
				{
					Crosshair = Weapon->GetCrosshair();

					if (InstWeapon)
					{
						CurrentSpread = 300 * FMath::Tan(FMath::DegreesToRadians(InstWeapon->GetCurrentSpread()));
					}

					if (Crosshair)
					{
						DynMat = UMaterialInstanceDynamic::Create(Crosshair, this);

						if (DynMat)
						{
							DynMat->SetScalarParameterValue("Cross_Radius", CurrentSpread);
							DynMat->SetScalarParameterValue("Square_Radius", CurrentSpread);
							PCOwner->GetViewportSize(ViewportSizeX, ViewportSizeY);
							auto ScreenLocation = FVector2D(ViewportSizeX * 0.5F, ViewportSizeY * 0.5f);
							DrawMaterial(DynMat, ScreenLocation.X - ScreenLocation.Y, 0.0f, ViewportSizeY, ViewportSizeY, 0.f, 0.f, 1.f, 1.f);
						}
					}
				}
				else if (Pawn->IsAiming() && Weapon->GetAimDisplay())
				{
					DrawAimCrosshair(Weapon->GetAimDisplay());
				}
			}
		}
	}
}

void ALethalEntertainmentHUD::DrawPauseUI()
{
	ACharacterPlayerController * PC = Cast<ACharacterPlayerController>(PlayerOwner);

	if (PC)
	{
		PC->IsPaused() ? PC->SetPause(false) : PC->SetPause(true);
		DrawPauseMenu(PC->IsPaused());
	}
}

void ALethalEntertainmentHUD::DrawGameOverUI()
{
	DrawGameOverMenu();
}

