// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapons.h"
#include "Weapon_ProjectileFire.generated.h"


class AWeaponProjectile;

UCLASS()
class LETHALENTERTAINMENT_API AWeapon_ProjectileFire : public AWeapons
{
	GENERATED_BODY()

protected:
	/** Projectile used by weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<AWeaponProjectile> ProjectileBP;

	/** Speed of projectile movment */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 3000, ClampMax = 10000))
	float FireVelocity;

protected:
	/** Weapon fire behaviour */
	virtual void FireWeapon() override;

private:
	/** Spawns projectile */
	void FireProjectile(FVector & Origin, FVector & AimDir);
	
};

