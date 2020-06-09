// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UI/Menu/MenuWidget.h"
#include "LethalSaveGame.generated.h"

enum class EControlType : uint8;

/**
 * Saves User settings and Game Data
 */
UCLASS()
class LETHALENTERTAINMENT_API ULethalSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	/** Player default invert Y axis setting */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Player Invert Y Axis", Category = "Config")
	bool Saved_InvertY = false;

	/** Player default controller vibration setting */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Player Controller Vibration", Category = "Config")
	bool Saved_Vibration = true;

	/** Player default mouse sensitivity */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Player Mouse Sensitivity", Category = "Config", meta = (ClampMin = 1.f, ClampMax = 100.f))
	float Saved_MouseSens = 50.f;

	/** Player default mouse ADS sensitivity */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Player Mouse ADS Sensitivity", Category = "Config", meta = (ClampMin = 1.f, ClampMax = 100.f))
	float Saved_ADS_MouseSens = 50.f;

	/** Player default controller sensitivity */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Player Controller Sensitivity", Category = "Config", meta = (ClampMin = 1.f, ClampMax = 100.f))
	float Saved_ConSens = 50.f;

	/** Player default controller ADS sensitivity */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Player Controller ADS Sensitivity", Category = "Config", meta = (ClampMin = 1.f, ClampMax = 100.f))
	float Saved_ADS_ConSens = 50.f;

	/** Highscore */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Highscore", Category = "Config", meta = (ClampMin = 0.f))
	int32 Saved_Highscore = 0;

	/** Control Type */
	UPROPERTY(EditDefaultsOnly, DisplayName = "Control Type", Category = "Config")
	EControlType Saved_ControlType = EControlType::MouseAndKeyboard;

};
