// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FX/ExplosionFX.h"
#include "WeaponProjectile.h"


AWeaponProjectile::AWeaponProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<USphereComponent>(FName("Collision Box"));
	CollisionBox->InitSphereRadius(8.0f);
	SetRootComponent(CollisionBox);

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	CollisionMesh->SetupAttachment(RootComponent);
	CollisionMesh->SetVisibility(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(FName("Projectile FX"));
	ParticleComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ParticleComp->bAutoActivate = true;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("Audio FX"));
	AudioComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	AudioComp->bAutoActivate = true;
}

void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	InitialLifeSpan = ProjectileData.DestroyTimer;

	FTimerHandle TimerDetonate;

	GetWorld()->GetTimerManager().SetTimer(TimerDetonate, this, &AWeaponProjectile::OnDetonate, ProjectileData.DetonateTimer, false);
}

void AWeaponProjectile::InitVelocity(float InitVelocity)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = InitVelocity;
	}
}

void AWeaponProjectile::ProjectileSurfaceTrace()
{
	FHitResult Hit;
	FVector EndTrace = RootComponent->GetComponentLocation() + FVector(0.f, 0.f, -100.f);
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(
		Hit,
		GetActorLocation(),
		EndTrace,
		ECC_Camera,
		TraceParams
	)
		)
	{
		SpawnExplosionFX(Hit);
	}
	else if (AirFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AirFX, GetActorLocation());
	}
}

void AWeaponProjectile::SpawnExplosionFX(const FHitResult & Impact)
{
	if (ExplosionFX && Impact.bBlockingHit)
	{
		FTransform const SpawnTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint);
		AExplosionFX * const FXActor = GetWorld()->SpawnActorDeferred<AExplosionFX>(ExplosionFX, SpawnTransform);
		if (FXActor)
		{
			FXActor->SurfaceHit = Impact;
			UGameplayStatics::FinishSpawningActor(FXActor, SpawnTransform);
		}
	}
	else if (DefaultFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultFX, GetActorLocation());
	}
}

void AWeaponProjectile::OnDetonate()
{
	CollisionMesh->DestroyComponent();
	ParticleComp->Deactivate();
	AudioComp->Deactivate();
	ProjectileMovement->Deactivate();

	ProjectileSurfaceTrace();

	UGameplayStatics::ApplyRadialDamage(
		this,
		ProjectileData.Damage,
		GetActorLocation(),
		ProjectileData.ExplosionRadius,
		ProjectileData.DamageType,
		TArray<AActor*>(),
		this,
		Instigator->GetInstigatorController(),
		false,
		COLLISION_EXPLOSION
	);

	if (ExplosionSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
	if (ExplosionCameraShakeBP)
	{
		UGameplayStatics::PlayWorldCameraShake(this, ExplosionCameraShakeBP, GetActorLocation(), 300.f, 1000.f, 1.f);
	}

	FTimerHandle TimerDestroy;
	GetWorld()->GetTimerManager().SetTimer(TimerDestroy, this, &AWeaponProjectile::OnDestroy, ProjectileData.DestroyTimer, false);
}

void AWeaponProjectile::OnDestroy()
{
	Destroy();
}

