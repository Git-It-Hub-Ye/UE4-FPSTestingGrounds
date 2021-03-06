// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosionFX.generated.h"

UCLASS()
class LETHALENTERTAINMENT_API AExplosionFX : public AActor
{
	GENERATED_BODY()
	
public:
	///////////////////////////////////////////////////////////////////////////////////
	// Particle effects

	/** Impact fx on default */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * DefaultFX;

	/** Impact fx on dirt */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * DirtFX;

	/** Impact fx on metal */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * MetalFX;


	////////////////////////////////////////////////////////////////
	// Get fx from surface

	/** SurfaceData for spawning */
	UPROPERTY(BlueprintReadOnly, Category = Surface)
	FHitResult SurfaceHit;

public:	
	/** Spawn effect */
	virtual void PostInitializeComponents() override;

protected:
	/** Get fx for material type */
	UFUNCTION()
	virtual UParticleSystem * GetExplosionFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const;

};
