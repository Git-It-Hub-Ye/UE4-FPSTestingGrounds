// Copyright 2018 - 2021 Stuart McDonald.

#include "LethalEntertainment.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ImpactFX.h"

void AImpactFX::PostInitializeComponents()
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

	// Destroy actor
	Destroy();
}

UParticleSystem * AImpactFX::GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	UParticleSystem * ImpactFX = nullptr;

	switch (SurfaceType)
	{
		case SURFACE_BODYSENSITIVE:
		case SURFACE_BODYVULNERABLE:
		case SURFACE_BODY:            ImpactFX = BodyFX;		  break;
		case SURFACE_CONCRETE:        ImpactFX = ConcreteFX;	  break;
		case SURFACE_METAL:           ImpactFX = MetalFX;		  break;
		case SURFACE_ROCK:            ImpactFX = RockFX;		  break;
		case SURFACE_SAND:            ImpactFX = SandFX;		  break;
		case SURFACE_PLASTIC:		  ImpactFX = PlasticFX;		  break;
		case SURFACE_DIRT:            ImpactFX = DirtFX;		  break;
		case SURFACE_SOFTMETAL:       ImpactFX = SoftMetalFX;	  break;
		case SURFACE_BARRIERCLOSED:   ImpactFX = BarrierClosedFX; break;
		case SURFACE_BARRIEROPEN:     ImpactFX = BarrierOpenFX;	  break;
		default:					  ImpactFX = DefaultFX;		  break;
	}

	return ImpactFX;
}

USoundBase * AImpactFX::GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	USoundBase * ImpactSound = nullptr;

	switch (SurfaceType)
	{
		case SURFACE_BODYSENSITIVE:
		case SURFACE_BODYVULNERABLE:
		case SURFACE_BODY:             ImpactSound = BodySound;		 break;
		case SURFACE_CONCRETE:         ImpactSound = ConcreteSound;  break;
		case SURFACE_METAL:            ImpactSound = MetalSound;	 break;
		case SURFACE_ROCK:             ImpactSound = RockSound;		 break;
		case SURFACE_SAND:             ImpactSound = SandSound;		 break;
		case SURFACE_PLASTIC:          ImpactSound = PlasticSound;	 break;
		case SURFACE_DIRT:             ImpactSound = DirtSound;		 break;
		case SURFACE_SOFTMETAL:        ImpactSound = SoftMetalSound; break;
		case SURFACE_BARRIEROPEN:
		case SURFACE_BARRIERCLOSED:    ImpactSound = BarrierSound;	 break;
		default:					   ImpactSound = DefaultSound;	 break;
	}

	return ImpactSound;
}

UMaterial * AImpactFX::GetImpactDecal(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	UMaterial * ImpactDecal = nullptr;

	switch (SurfaceType)
	{
		case SURFACE_CONCRETE:			ImpactDecal = ConcreteDecal.DecalMaterial;
										DecalSize = ConcreteDecal.DecalSize;
										DecalLifeSpan = ConcreteDecal.LifeSpan;		break;
		case SURFACE_METAL:				ImpactDecal = MetalDecal.DecalMaterial;
										DecalSize = MetalDecal.DecalSize;
										DecalLifeSpan = MetalDecal.LifeSpan;		break;
		case SURFACE_SAND:				ImpactDecal = SandDecal.DecalMaterial;
										DecalSize = SandDecal.DecalSize;
										DecalLifeSpan = SandDecal.LifeSpan;			break;
		case SURFACE_PLASTIC:			ImpactDecal = PlasticDecal.DecalMaterial;
										DecalSize = PlasticDecal.DecalSize;
										DecalLifeSpan = PlasticDecal.LifeSpan;		break;
		case SURFACE_SOFTMETAL:			ImpactDecal = SoftMetalDecal.DecalMaterial;
										DecalSize = SoftMetalDecal.DecalSize;
										DecalLifeSpan = SoftMetalDecal.LifeSpan;	break;
		default:						ImpactDecal = DefaultDecal.DecalMaterial;
										DecalSize = DefaultDecal.DecalSize;
										DecalLifeSpan = DefaultDecal.LifeSpan;		break;
	}

	return ImpactDecal;
}

