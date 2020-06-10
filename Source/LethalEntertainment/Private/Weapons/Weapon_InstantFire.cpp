// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Player/Mannequin.h"
#include "FX/ImpactFX.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapon_InstantFire.h"


//////////////////////////////////////////////////////////////////////////
// Weapon usage

AWeapon_InstantFire::AWeapon_InstantFire()
{
	CurrentFiringSpread = 0.f;
}

void AWeapon_InstantFire::FireWeapon()
{
	Super::FireWeapon();

	if (BulletType == EWeaponBulletType::EBullet)
	{
		InstantFire();
	}
	if (BulletType == EWeaponBulletType::ESpread)
	{
		for (int32 i = 0; i < 10; i++)
		{
			InstantFire();
		}
	}
}

void AWeapon_InstantFire::InstantFire()
{
	const int32 RandSeed = FMath::Rand();
	FRandomStream RandStream(RandSeed);
	const float CurrentSpread = GetCurrentSpread();
	const float SpreadCone = FMath::DegreesToRadians(CurrentSpread * 0.5);
	const FVector AimDir = GetAdjustedAim();
	const FVector StartTrace = StartTraceVectorOnFire(AimDir);

	const FVector ShootDirection = RandStream.VRandCone(AimDir, SpreadCone, SpreadCone);
	const FVector EndTrace = StartTrace + (ShootDirection * InstantWeaponConfig.WeaponRange);
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	ProcessInstantHit(Impact, StartTrace, ShootDirection, RandSeed, CurrentSpread);

	CurrentFiringSpread = FMath::Min(InstantWeaponConfig.MaxWeaponSpread, CurrentFiringSpread + InstantWeaponConfig.WeaponSpreadIncrement);
}

void AWeapon_InstantFire::ProcessInstantHit(const FHitResult & Impact, const FVector & Origin, const FVector & ShootDir, int32 RandSeed, float ReticuleSpread)
{
	const FVector EndTrace = Origin + ShootDir * (InstantWeaponConfig.WeaponRange);
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;

	SpawnTracerFX(EndPoint);
	SpawnImpactEffect(Impact);

	UAISense_Hearing::ReportNoiseEvent(this, Impact.ImpactPoint, 1.f, Instigator, 800.f);

	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Impact.PhysMaterial.Get());

	AMannequin * HitActor = Cast<AMannequin>(Impact.GetActor());

	if (CharacterPawn && HitActor && CharacterPawn->GetCharacterType() == HitActor->GetCharacterType()) { return; }
	if (HitActor)
	{
		float ActualDamage = GetActualDamage(SurfaceType);

		if (CharacterPawn)
		{
			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShootDir, Impact, CharacterPawn->GetInstigatorController(), this, DamageType);
			UAISense_Damage::ReportDamageEvent(this, HitActor, Instigator, ActualDamage, CharacterPawn->GetActorLocation(), Impact.Location);
		}
	}
}

void AWeapon_InstantFire::OnFireFinished()
{
	Super::OnFireFinished();
	CurrentFiringSpread = 0.f;
}


//////////////////////////////////////////////////////////////////////////
// Reading data

float AWeapon_InstantFire::GetCurrentSpread() const
{
	float Spread = InstantWeaponConfig.WeaponSpread;
	bool bIsCrouched = false;
	bool bIsMoving = false;

	if (CharacterPawn)
	{
		if (CharacterPawn->IsCrouching())
		{
			Spread = InstantWeaponConfig.CrouchingSpread;
			bIsCrouched = true;
			if (CharacterPawn->IsMoving())
			{
				Spread += InstantWeaponConfig.AddMovementSpread / 2;
				bIsMoving = true;
			}
		}
		else if (CharacterPawn->IsMoving())
		{
			Spread += InstantWeaponConfig.AddMovementSpread;
			bIsMoving = true;
		}
	}

	float FinalSpread = Spread + CurrentFiringSpread;

	if (bIsCrouched)
	{
		if (bIsMoving && FinalSpread > InstantWeaponConfig.MaxCrouchedSpread)
		{
			FinalSpread = InstantWeaponConfig.MaxCrouchedSpread;
		}
		else if (FinalSpread > InstantWeaponConfig.CrouchWhileStillMax)
		{
			FinalSpread = InstantWeaponConfig.CrouchWhileStillMax;
		}
	}
	else if (!bIsCrouched)
	{
		if (bIsMoving && FinalSpread > InstantWeaponConfig.MaxWeaponSpread)
		{
			FinalSpread = InstantWeaponConfig.MaxWeaponSpread;
		}
		else if (FinalSpread > InstantWeaponConfig.StandWhileStillMax)
		{
			FinalSpread = InstantWeaponConfig.StandWhileStillMax;
		}
	}

	if (CharacterPawn && CharacterPawn->IsPlayerControlled() && CharacterPawn->IsAiming())
	{
		FinalSpread = InstantWeaponConfig.AimingSpread;
	}
	return FinalSpread;
}


//////////////////////////////////////////////////////////////////////////
// FX

void AWeapon_InstantFire::SpawnImpactEffect(const FHitResult & Impact)
{
	if (ImpactEffects && Impact.bBlockingHit)
	{
		FHitResult UseImpact = Impact;
		
		FTransform const SpawnTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint);
		AImpactFX * Effect = GetWorld()->SpawnActorDeferred<AImpactFX>(ImpactEffects, SpawnTransform);

		if (Effect)
		{
			Effect->SurfaceHit = UseImpact;
			UGameplayStatics::FinishSpawningActor(Effect, SpawnTransform);
		}
	}
}

void AWeapon_InstantFire::SpawnTracerFX(const FVector & EndPoint)
{
	if (TracerFX)
	{
		FVector SpawnStart = CharacterPawn && CharacterPawn->IsPlayerControlled() && CharacterPawn->IsAiming() ? StartTraceVectorOnFire(AimDirction()) : GetMuzzleLocation();

		UParticleSystemComponent * TracerComp = UGameplayStatics::SpawnEmitterAtLocation(this, TracerFX, SpawnStart);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetParam, EndPoint);
		}
	}
}

float AWeapon_InstantFire::GetActualDamage(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	float ActualDamage = InstantWeaponConfig.BaseDamage;
	if (SurfaceType == SURFACE_BODYVULNERABLE)
	{
		ActualDamage = InstantWeaponConfig.HighDamage;
	}
	else if (SurfaceType == SURFACE_BODYSENSITIVE)
	{
		ActualDamage = InstantWeaponConfig.MidDamage;
	}
	return ActualDamage;
}

