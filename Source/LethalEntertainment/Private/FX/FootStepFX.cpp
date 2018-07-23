// Fill out your copyright notice in the Description page of Project Settings.

#include "LethalEntertainment.h"
#include "FootStepFX.h"

USoundBase * AFootStepFX::GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	USoundBase * ImpactSound = nullptr;
	switch (SurfaceType)
	{
	case SURFACE_DIRT:	ImpactSound = DirtSound; break;
	case SURFACE_METAL: ImpactSound = MetalSound; break;
	case SURFACE_ROCK:	ImpactSound = RockSound; break;
	case SURFACE_SAND:	ImpactSound = SandSound; break;
	default:			ImpactSound = DefaultSound; break;
	}
	return ImpactSound;
}

