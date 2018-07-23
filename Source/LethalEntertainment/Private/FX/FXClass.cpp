// Fill out your copyright notice in the Description page of Project Settings.

#include "LethalEntertainment.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "FXClass.h"


AFXClass::AFXClass()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFXClass::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPhysicalMaterial * HitPhysMat = SurfaceHit.PhysMaterial.Get();
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);

	/** Spawn impact fx */
	UParticleSystem * ImpactFX = GetImpactFX(HitSurfaceType);
	if (ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());
	}

	/** Spawn impact sound */
	USoundBase * ImpactSound = GetImpactSound(HitSurfaceType);
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	/** Spawn impact decal */
	UMaterial * ImpactDecal = GetImpactDecal(HitSurfaceType);
	if (ImpactDecal)
	{
		FRotator RandRotation = SurfaceHit.ImpactNormal.Rotation();
		RandRotation.Roll = FMath::FRandRange(-180.f, 180.f);

		UGameplayStatics::SpawnDecalAttached(ImpactDecal, FVector(1.0f, DecalSize, DecalSize),
			SurfaceHit.Component.Get(), SurfaceHit.BoneName,
			SurfaceHit.ImpactPoint, RandRotation,
			EAttachLocation::KeepWorldPosition,
			DecalLifeSpan);
	}

	/** Remove actor */
	Destroy();
}

UParticleSystem * AFXClass::GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	// Default base
	return nullptr;
}

USoundBase * AFXClass::GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	// Default base
	return nullptr;
}

UMaterial * AFXClass::GetImpactDecal(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	// Default base
	return nullptr;
}

