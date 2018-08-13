// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "WeaponProjectile.h"
#include "Weapon_ProjectileFire.h"


void AWeapon_ProjectileFire::FireWeapon()
{
	FVector AimDir = GetAdjustedAim();
	FVector Origin = GetMuzzleLocation();

	const FVector StartTrace = StartTraceVectorOnFire(AimDir);
	const FVector EndTrace = StartTrace + AimDir * 10000.f;
	FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	if (Impact.bBlockingHit)
	{
		const FVector AdjustedDir = (Impact.ImpactPoint - Origin).GetSafeNormal();

		AimDir = AdjustedDir;
	}

	FireProjectile(Origin, AimDir);
}

void AWeapon_ProjectileFire::FireProjectile(FVector & Origin, FVector & AimDir)
{
	FTransform SpawnT(AimDir.Rotation(), Origin);

	AWeaponProjectile * Projectile = Cast<AWeaponProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileBP, SpawnT));
	if (Projectile)
	{
		Projectile->Instigator = Instigator;
		Projectile->SetOwner(this);
		Projectile->InitVelocity(FireVelocity);
		
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnT);
	}
}

