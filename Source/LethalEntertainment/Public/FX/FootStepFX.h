// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FootStepFX.generated.h"

UCLASS()
class LETHALENTERTAINMENT_API AFootStepFX : public AActor
{
	GENERATED_BODY()
	
public:
	///////////////////////////////////////////////////////////////////////////////////
	// Sound effects

	/** Sound fx on default */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * DefaultSound;

	/** Sound fx on dirt */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * DirtSound;

	/** Sound fx on rock */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * RockSound;

	/** Sound fx on metal */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * MetalSound;

	/** Sound fx on sand */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * SandBagSound;


	////////////////////////////////////////////////////////////////
	// Get fx from surface

	/** SurfaceData for spawning */
	UPROPERTY(BlueprintReadOnly, Category = Surface)
	FHitResult SurfaceHit;

public:	
	/** Spawn effect */
	virtual void PostInitializeComponents() override;

protected:
	/** Get sound for material type */
	UFUNCTION()
	virtual USoundBase * GetFootStepSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const;
	
};
