// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.generated.h"

class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundBase;
class UCameraShake;
class AExplosionFX;

USTRUCT(BlueprintType)
struct FWeaponProjectileData {
	GENERATED_USTRUCT_BODY()

	/* Max damage (damage dealt at close range).*/
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 200))
	float Damage = 0;

	/** Radius of explosion */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 2000))
	float ExplosionRadius = 0;

	/* Time till detonation */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 10))
	float DetonateTimer = 0;

	/* Time till actor is destroyed.*/
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 10))
	float DestroyTimer = 0;

	/** Damage type */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

	/** Defaults */
	FWeaponProjectileData()
	{
		Damage = 100;
		ExplosionRadius = 1000;
		DetonateTimer = 5.f;
		DestroyTimer = 10.f;
		DamageType = UDamageType::StaticClass();
	}

};


UCLASS()
class LETHALENTERTAINMENT_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()

protected:
	////////////////////////////////////////////////////////////////////////////////
	// Reading data

	/** Projectile data */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FWeaponProjectileData ProjectileData;

private:
	////////////////////////////////////////////////////////////////////////////////
	// FX

	/** Camera shake on explosion */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TSubclassOf<UCameraShake> ExplosionCameraShakeBP;

	/** Particle component for projectile fx */
	UPROPERTY(VisibleDefaultsOnly, Category = "FX")
	UParticleSystemComponent * ParticleComp;

	/** Audio component for projectile fx */
	UPROPERTY(VisibleDefaultsOnly, Category = "FX")
	UAudioComponent * AudioComp;

	/** Explosion sound FX */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	USoundBase * ExplosionSound;

	/** Explosion FX */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TSubclassOf<AExplosionFX> ExplosionFX;

	/** Air explosion FX */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem * AirFX;

	/** Default explosion FX */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem * DefaultFX;



	////////////////////////////////////////////////////////////////////////////////
	// Mesh

	/** Projectiles collision box */
	UPROPERTY(VisibleDefaultsOnly, Category = "Config")
	USphereComponent * CollisionBox;

	/** Projectile mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = "Config")
	UStaticMeshComponent * CollisionMesh;

	/** Projectile movement component */
	UPROPERTY(VisibleDefaultsOnly, Category = "Movement")
	UProjectileMovementComponent * ProjectileMovement;
	
public:
	AWeaponProjectile();

	/** setup velocity */
	void InitVelocity(float InitVelocity);

protected:
	virtual void BeginPlay() override;

	/** Get surface for explosion fx */
	void ProjectileSurfaceTrace();

	/** Play explosion fx */
	void SpawnExplosionFX(const FHitResult & Impact);

private:
	/** Detonation behaviour */
	void OnDetonate();

	/** Destroys projectile */
	void OnDestroy();
};
