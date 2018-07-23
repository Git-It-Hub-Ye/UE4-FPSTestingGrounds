// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "LethalEntertainmentHUD.generated.h"

UCLASS()
class ALethalEntertainmentHUD : public AHUD
{
	GENERATED_BODY()

protected:
	/** Weapon crosshair material */
	UMaterialInstance * Crosshair;

	/** Crosshair dynamic material for spread */
	UMaterialInstanceDynamic * DynMat;

	/** For crosshair to display on player monitor, for both X/Y coordinates to avoid stretching of material */
	int32 ViewportSizeX, ViewportSizeY;

	/** Current spread of crosshair */
	float CurrentSpread;

public:
	ALethalEntertainmentHUD();

	/** Main hud loop */
	virtual void DrawHUD() override;

	/** Draws game over menu on screen */
	void DrawGameOverUI();

	/** Draws pause menu on screen */
	void DrawPauseUI();

protected:
	/** Show crosshair widget blueprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "Hud")
	void DrawAimCrosshair(UUserWidget * AimDispaly);

	/** Show pause widget blueprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "Hud")
	void DrawPauseMenu(bool bDrawUI);

	/** Show game over widget blueprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "Hud")
	void DrawGameOverMenu();

private:
	/** Draws crosshair on screen */
	void DrawCrosshair();

};

