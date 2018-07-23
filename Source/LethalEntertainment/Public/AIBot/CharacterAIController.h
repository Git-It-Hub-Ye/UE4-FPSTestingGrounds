// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CharacterAIController.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;
class AMannequin;

/** Range of senses */
USTRUCT(BlueprintType)
struct FSenseData {
	GENERATED_USTRUCT_BODY()

	/** Sight range */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 100, ClampMax = 3000))
	float SightRange;

	/** Lost sight range */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 200, ClampMax = 3000))
	float LostSightRange;

	/** Peripheral vision angle from centre to each side */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 40, ClampMax = 90))
	float PeripheralVisionAngle;

	/** Hearing range */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 100, ClampMax = 5000))
	float HearingRange;

	/** Defaults */
	FSenseData()
	{
		SightRange = 2000.f;
		LostSightRange = 2500.f;
		PeripheralVisionAngle = 80.f;
		HearingRange = 3000.f;
	}
};

UCLASS()
class LETHALENTERTAINMENT_API ACharacterAIController : public AAIController
{
	GENERATED_BODY()

protected:
	/** Blackboard to use */
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	UBlackboardData * BlackboardToUse;

	/** Behaviour tree to use */
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	UBehaviorTree * BehaviorTreeToUse;

	/** Will be a return value */
	UPROPERTY()
	UBlackboardComponent * BB;

	/** Ai perception component */
	UPROPERTY(VisibleAnywhere, Category = "AIPerception")
	UAIPerceptionComponent * PerceptionComp;

	/** Data for senses */
	UPROPERTY(EditDefaultsOnly, Category = "AIPerception")
	FSenseData SenseData;

	/** Ai sight sense */
	UAISenseConfig_Sight * SightConfig;

	/** Ai hearing sense */
	UAISenseConfig_Hearing * HearingConfig;

	/** Ai Damage sense */
	UAISenseConfig_Damage * DamageConfig;

	FVector SensedPos;

	/** Team id */
	FGenericTeamId TeamId;

	/** Key for enemy value */
	FName EnemyKeyId;

	/** Key for last known location value*/
	FName LastKnownLocationKeyId;

	/** Key for bool value */
	FName CanShootKeyId;

public:
	ACharacterAIController(const FObjectInitializer & ObjectInitializer);

	/** Returns team id */
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	/** Checks if sensed actor is friend or foe */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor & Other) const override;

	/** Fires weapon if possible */
	void ShootEnemy();

	/** Stops weapon fire */
	void StopFire();

	/** Aim down weapon sight */
	void AimWeapon(bool bAimWeapon);

protected:
	/** sets the ai controlled pawn */
	virtual void SetPawn(APawn * InPawn) override;

	/** Checks if ai and enemy are on the same tile */
	bool IsOnSameTile(AActor * Actor);

	/** Destroys ai controller when character health is zero */
	UFUNCTION()
	void OnPossessedCharacterDeath();

	/** Called when actors are sensed */
	UFUNCTION()
	void SenseStuff(AActor * SensedActor, FAIStimulus Stimulus);

	/** Updates last location of enemy */
	UFUNCTION(BlueprintCallable, Category = "AIBehaviour")
	void UpdateLastKnownLocation();

};
