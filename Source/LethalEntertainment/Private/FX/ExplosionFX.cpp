// Fill out your copyright notice in the Description page of Project Settings.

#include "LethalEntertainment.h"
#include "ExplosionFX.h"


UParticleSystem * AExplosionFX::GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	UParticleSystem * ImpactFX = nullptr;
	switch (SurfaceType)
	{
	case SURFACE_METAL:			ImpactFX = MetalFX; break;
	case SURFACE_DIRT:			ImpactFX = DirtFX; break;
	default:					ImpactFX = DefaultFX; break;
	}

	return ImpactFX;
}

