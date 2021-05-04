// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Runtime/AIModule/Classes/GenericTeamAgentInterface.h"
#include "CharacterPlayerController.generated.h"

class ALethalEntertainmentHUD;

UCLASS()
class LETHALENTERTAINMENT_API ACharacterPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	/** Controller vibration */
	bool bVibrateController = true;

protected:
	/** Team id */
	FGenericTeamId TeamId;

private:
	/** New target rotation for controller */
	float NewTargetRot;

	/** Input along x-axis */
	float DeltaX;

	/** Input along y-axis */
	float DeltaY;

public:
	/** Set defaults and components */
	ACharacterPlayerController(const FObjectInitializer & ObjectInitializer);

	/** Setup player input actions */
	virtual void SetupInputComponent() override;

	void FindDeathCameraSpot(FVector & StartLocation);

	void BlendToDeathCamera(FVector CamLoc, FRotator CamRot);

	/** Gets player team id */
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	/** Gets target rotation for controller after player input along x/y axis */
	float CalculateTargetRotaion(float CurrentRot, float CurrentTarget);

	/** Get the hud */
	ALethalEntertainmentHUD * GetPlayerHud() const;

	/** Return bVibrateController */
	bool IsVibrationEnabled() const { return bVibrateController; }

protected:
	/** Unpossess controller on death */
	UFUNCTION()
	void OnPossessedCharacterDeath();

private:
	/** Set controller pawn */
	virtual void SetPawn(APawn * InPawn) override;

	/** Show pause game menu from hud*/
	void PauseGameMenu();

};
