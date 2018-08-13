// Copyright 2018 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpactFX.generated.h"

/** Data of decal spawned */
USTRUCT()
struct FDecalData {
	GENERATED_USTRUCT_BODY()

	/** Material to show on impact */
	UPROPERTY(EditDefaultsOnly, Category = Decals)
	UMaterial * DecalMaterial;

	/** Size of decal */
	UPROPERTY(EditDefaultsOnly, Category = Decals)
	float DecalSize = 15.f;

	/** How long decal will be visible */
	UPROPERTY(EditDefaultsOnly, Category = Decals)
	float LifeSpan = 5.f;
};

UCLASS()
class LETHALENTERTAINMENT_API AImpactFX : public AActor
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

	/** Impact fx on concrete */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * ConcreteFX;

	/** Impact fx on rock */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * RockFX;

	/** Impact fx on metal */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * MetalFX;

	/** Impact fx on plastic */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * PlasticFX;

	/** Impact fx on sand */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * SandFX;

	/** Impact fx on body */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * BodyFX;

	/** Impact fx on soft metals */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * SoftMetalFX;

	/** Impact fx on barrier */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * BarrierClosedFX;

	/** Impact fx on barrier */
	UPROPERTY(EditDefaultsOnly, Category = ParticleEffects)
	UParticleSystem * BarrierOpenFX;


	///////////////////////////////////////////////////////////////////////////////////
	// Sound effects

	/** Sound fx on default */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * DefaultSound;

	/** Sound fx on dirt */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * DirtSound;

	/** Sound fx on concrete */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * ConcreteSound;

	/** Sound fx on rock */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * RockSound;

	/** Sound fx on metal */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * MetalSound;

	/** Sound fx on plastic */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * PlasticSound;

	/** Sound fx on sand */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * SandSound;

	/** Sound fx on body */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * BodySound;

	/** Sound fx on soft metals */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * SoftMetalSound;

	/** Sound fx on barrier */
	UPROPERTY(EditDefaultsOnly, Category = SoundEffects)
	USoundBase * BarrierSound;


	////////////////////////////////////////////////////////////////
	// Decals for impacts

	/** Decal for concrete */
	UPROPERTY(EditDefaultsOnly, Category = Decals)
	FDecalData ConcreteDecal;

	/** Decal for metal */
	UPROPERTY(EditDefaultsOnly, Category = Decals)
	FDecalData MetalDecal;

	/** Decal for soft metal */
	UPROPERTY(EditDefaultsOnly, Category = Decals)
	FDecalData SoftMetalDecal;

	/** Decal for plastic */
	UPROPERTY(EditDefaultsOnly, Category = Decals)
	FDecalData PlasticDecal;

	/** Decal for sandbags */
	UPROPERTY(EditDefaultsOnly, Category = Decals)
	FDecalData SandDecal;

	/** Default decal */
	UPROPERTY(EditDefaultsOnly, Category = Decals)
	FDecalData DefaultDecal;


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
	virtual UParticleSystem * GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const;

	/** Get sound for material type */
	UFUNCTION()
	virtual USoundBase * GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const;

	/** Get decal for material type */
	UFUNCTION()
	virtual UMaterial * GetImpactDecal(TEnumAsByte<EPhysicalSurface> SurfaceType);

protected:
	/** Size of decal */
	float DecalSize;

	/** Lifespan of decal */
	float DecalLifeSpan;

};
