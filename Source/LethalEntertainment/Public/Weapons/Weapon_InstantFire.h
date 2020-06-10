// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapons.h"
#include "Weapon_InstantFire.generated.h"


class AImpactFX;

 /** What does the weapon fire (eg. Bullet-Rifle, Spread-ShotGun) */
UENUM()
enum class EWeaponBulletType : uint8 {
	EBullet			UMETA(DisplayName = "Bullet"),
	ESpread			UMETA(DisplayName = "Spread"),
};

/** Instant weapon configuration */
USTRUCT(BlueprintType)
struct FWeaponConfig {
	GENERATED_USTRUCT_BODY()

	/** Length of weapons line trace */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1000, ClampMax = 1000))
	float WeaponRange;

	/** Default acurracy of line trace from start to end point */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 10))
	float WeaponSpread;

	/** Default acurracy of line trace from start to end point while crouched */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 10))
	float CrouchingSpread;

	/** Max spread while crouched and not moving */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 20))
	float CrouchWhileStillMax;

	/** Max spread while standing still */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 20))
	float StandWhileStillMax;

	/** Max spread while crouched */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 20))
	float MaxCrouchedSpread;

	/** Max spread */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 20))
	float MaxWeaponSpread;

	/** Spread to add while moving */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 20))
	float AddMovementSpread;

	/** Aiming spread */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 0, ClampMax = 10))
	float AimingSpread;

	/** Continuous firing spread increment */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 10))
	float WeaponSpreadIncrement;

	/** Base damage dealt each hit on base phys material */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 100))
	float BaseDamage;

	/** Mid damage dealt each hit on sensitive phys material */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 100))
	float MidDamage;

	/** High damage dealt each hit on vulnerable phys material */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 100))
	float HighDamage;

	/** Defaults */
	FWeaponConfig()
	{
		WeaponRange = 10000;
		WeaponSpread = 6.f;
		CrouchingSpread = 4.f;
		CrouchWhileStillMax = 8.f;
		StandWhileStillMax = 12.f;
		MaxCrouchedSpread = 10.f;
		MaxWeaponSpread = 15.f;
		WeaponSpreadIncrement = 1.f;
		AddMovementSpread = 10.f;
		AimingSpread = 0.f;
		BaseDamage = 5.f;
		MidDamage = 10.f;
		HighDamage = 15.f;
	}
};

UCLASS()
class LETHALENTERTAINMENT_API AWeapon_InstantFire : public AWeapons
{
	GENERATED_BODY()

private:

	//////////////////////////////////////////////////////////////////////////
	// Reading data

	/** Weapon fire type */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	EWeaponBulletType BulletType;

	/** Weapon damage type */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<UDamageType> DamageType;

	/** Struct variable which determines weapon behaviour */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FWeaponConfig InstantWeaponConfig;

	/** Spread of continuous firing */
	float CurrentFiringSpread;


	//////////////////////////////////////////////////////////////////////////
	// FX

	/** FX for tracer */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem * TracerFX;

	/** Param name for beam target in tracer fx */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	FName TracerTargetParam;

	/** Impact effects for different surfaces */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TSubclassOf<AImpactFX> ImpactEffects;
	
public:

	AWeapon_InstantFire();


	//////////////////////////////////////////////////////////////////////////
	// Reading data

	/** Get current firing spead */
	float GetCurrentSpread() const;

protected:

	//////////////////////////////////////////////////////////////////////////
	// Weapon usage

	virtual void FireWeapon() override;

	void InstantFire();

	void ProcessInstantHit(const FHitResult & Impact, const FVector & Origin, const FVector & ShootDir, int32 RandSeed, float ReticuleSpread);

	virtual void OnFireFinished() override;


	//////////////////////////////////////////////////////////////////////////
	// FX

	/** Get fx for surface type hit */
	void SpawnImpactEffect(const FHitResult & Impact);

	/** Spawn tracer fx */
	void SpawnTracerFX(const FVector& EndPoint);

	/** Get damage depending on surface type hit */
	float GetActualDamage(TEnumAsByte<EPhysicalSurface> SurfaceType);

};
