// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Weapons/Weapons.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "FPSCharacterMovement.h"
#include "CharacterPlayerController.h"
#include "AIBot/CharacterAIController.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"
#include "FX/FootStepFX.h"
#include "Terrain/Tile.h"
#include "LethalEntertainmentHUD.h"
#include "LethalGameInstance.h"
#include "Mannequin.h"


AMannequin::AMannequin(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFPSCharacterMovement>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(10.f, 1.75f, 64.f);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Mesh"));
	FPMesh->SetOnlyOwnerSee(true);
	FPMesh->SetupAttachment(FirstPersonCameraComponent);
	FPMesh->bCastDynamicShadow = false;
	FPMesh->CastShadow = false;
	FPMesh->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FPMesh->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	MuzzleEndLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleEndLocation"));
	MuzzleEndLocation->SetupAttachment(FPMesh);
	MuzzleEndLocation->SetRelativeLocation(FVector(30.f, 14.f, 138.f));

	HitTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Hit TimeLine"));
	HitInterpFunction.BindUFunction(this, FName("HitReactionTimeline"));
	HitTimelineFinished.BindUFunction(this, FName("ResetPhysicsOnTimelineFinished"));

	CrouchTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Crouch TimeLine"));
	CrouchInterpFunction.BindUFunction(this, FName("SmoothCrouchTimeline"));
	CrouchTimelineFinished.BindUFunction(this, FName("CrouchOnTimelineFinished"));

	CurrentWeapon = nullptr;

	bIsMovingForward = false;
	bIsMovingRight = false;
	bIsCrouching = false;
	bIsAiming = false;
	bWantsToAim = false;

	MouseSens_Modifier = 50.f;
	MouseSens_ADS_Modifier = 50.f;
	ConSens_Modifier = 50.f;
	ConSens_ADS_Modifier = 50.f;
	AimingSpeedModifier = 0.5;
	CrouchingSpeedModifier = 0.5;
}

void AMannequin::BeginPlay()
{
	Super::BeginPlay();

	if (IsPlayerControlled())
	{
		ULethalGameInstance * GI = GetWorld() ? Cast<ULethalGameInstance>(GetWorld()->GetGameInstance()) : nullptr;
		if (GI)
		{
			GI->GetCurrentUserValues(MouseSens_Modifier, MouseSens_ADS_Modifier, ConSens_Modifier, ConSens_ADS_Modifier);
			GI->OnUserSettingsUpdate.AddUniqueDynamic(this, &AMannequin::UpdateSensitivity);
		}
	}

	MouseBaseLookRate = MouseSens_Modifier;
	ConBaseLookRate = ConSens_Modifier;

	CurrentHealth = StartingHealth;
	StandHeight = GetDefaultHalfHeight();
	DefaultFOV = FirstPersonCameraComponent->FieldOfView;
	bIsDead = false;
	bIsHit = false;
	
	SpawnDefaultInventory();

	if (HitTimeline && FHitReactionCurve)
	{
		HitTimeline->AddInterpFloat(FHitReactionCurve, HitInterpFunction, FName("Alpha"));
		HitTimeline->SetTimelineFinishedFunc(HitTimelineFinished);
		HitTimeline->SetLooping(false);
	}
	if (CrouchTimeline && FCrouchCurve)
	{
		CrouchTimeline->AddInterpFloat(FCrouchCurve, CrouchInterpFunction, FName("Alpha"));
		CrouchTimeline->SetTimelineFinishedFunc(CrouchTimelineFinished);
		CrouchTimeline->SetLooping(false);
	}
	PlaySound(SpawnSound);
}

void AMannequin::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!bIsDead && MuzzleEndLocation)
	{
		IsWeaponBlocked();
	}
}

void AMannequin::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Inventory.Num() != NULL)
	{
		AWeapons * Weapons;

		while (Inventory.Num() != NULL)
		{
			Weapons = Inventory.Pop();

			if (Weapons != NULL)
			{
				Weapons->Destroy();
			}	
		}
	}

	Destroy();
}


////////////////////////////////////////////////////////////////////////////////////
// Mesh & Camera

bool AMannequin::CanBeSeenFrom(const FVector & ObserverLocation, FVector & OutSeenLocation, int32 & NumberOfLoSChecksPerformed, float & OutSightStrength, const AActor * IgnoreActor) const
{
	static const FName NAME_AILineOfSight = FName(TEXT("TestPawnLineOfSight"));

	FHitResult HitResult;
	
	const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, GetActorLocation()
		, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic))
		, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	NumberOfLoSChecksPerformed++;

	if (bHit == false || (HitResult.Actor.IsValid() && HitResult.Actor->IsOwnedBy(this)))
	{
		OutSeenLocation = GetActorLocation();
		OutSightStrength = 1;

		return true;
	}

	for (int i = 0; i < BodySockets.Num(); i++)
	{
		FVector SocketLocation = GetMesh()->GetSocketLocation(BodySockets[i]);

		const bool bHitSocket = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, SocketLocation
			, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic))
			, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

		NumberOfLoSChecksPerformed++;

		if (bHitSocket == false || (HitResult.Actor.IsValid() && HitResult.Actor->IsOwnedBy(this)))
		{
			OutSeenLocation = SocketLocation;
			OutSightStrength = 1;

			return true;
		}
	}

	OutSightStrength = 0;
	return false;
}


////////////////////////////////////////////////////////////////////////////////////
// Damage & Death

float AMannequin::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp<float>(DamagePoints, 0, CurrentHealth);

	if (!bIsDead)
	{
		if (!bIsHit && DamageEvent.GetTypeID() == FPointDamageEvent::ClassID)
		{
			FPointDamageEvent * PDamage = (FPointDamageEvent*)&DamageEvent;
			if (PDamage && PDamage->HitInfo.BoneName != FName("pelvis") && FHitReactionCurve)
			{
				PDamage->HitInfo.Location;
				bIsHit = true;
				HitBoneName = PDamage->HitInfo.BoneName;
				HitTimeline->PlayFromStart();
				GetMesh()->AddImpulse(PDamage->ShotDirection * 1200.f, HitBoneName, true);
			}
		}

		PlayDamagedEffects();

		CurrentHealth -= DamageToApply;

		if (CurrentHealth <= 0)
		{
			OnDeath();
		}
	}
	return DamageToApply;
}

void AMannequin::PlayDamagedEffects()
{
	ACharacterPlayerController * PC = Cast<ACharacterPlayerController>(this->GetController());

	if (PC)
	{
		CharacterDamaged();
		PC->ClientPlayCameraShake(DamageCamShakeBP);
	}
}

void AMannequin::HitReactionTimeline(float Value)
{
	GetMesh()->SetAllBodiesBelowSimulatePhysics(HitBoneName, true, true);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(HitBoneName, Value, false, true);
}

void AMannequin::ResetPhysicsOnTimelineFinished()
{
	GetMesh()->SetAllBodiesPhysicsBlendWeight(0.0f, false);
	GetMesh()->SetAllBodiesSimulatePhysics(false);
	bIsHit = false;
}

void AMannequin::AddHealth(int32 HealthToAdd)
{
	CurrentHealth = CurrentHealth + HealthToAdd;
	CurrentHealth = FMath::Clamp<int32>(CurrentHealth, 0, StartingHealth);
}

void AMannequin::OnDeath()
{
	bIsDead = true;
	HitTimeline->Stop();

	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->DestroyFPMesh();
		CurrentWeapon->StopActions();
	}

	StopAllAnimMontages();
	PlaySound(PowerDownSound);

	OnCharacterDeath.Broadcast();

	// Failsafe to avoid any interaction
	GetCapsuleComponent()->DestroyComponent();
	FPMesh->DestroyComponent();

	SetRagdollPhysics();
}

void AMannequin::SetRagdollPhysics()
{
	if (bIsDead)
	{
		GetMesh()->SetSimulatePhysics(true);
	}
}


////////////////////////////////////////////////////////////////////////////////////
// Inventory

void AMannequin::SpawnDefaultInventory()
{
	int32 NumOfWeapons = DefaultInventory.Num();

	for (int32 i = 0; i < NumOfWeapons; i++)
	{
		if (DefaultInventory[i])
		{
			AWeapons * NewWeapon = GetWorld()->SpawnActor<AWeapons>(DefaultInventory[i]);
			NewWeapon->SetOwningPawn(this);
			NewWeapon->OnUnEquip();
			Inventory.AddUnique(NewWeapon);
		}
	}

	if (Inventory.Num() > 0)
	{
		EquipWeapon(Inventory[0]);
	}
}

void AMannequin::EquipWeapon(AWeapons * Weapon)
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->OnUnEquip();
		CurrentWeapon = Weapon;
		Weapon->SetOwningPawn(this);
		Weapon->OnEquip();
	}
	else
	{
		CurrentWeapon = Weapon;
		Weapon->SetOwningPawn(this);
		Weapon->OnEquip();
	}
}

AWeapons * AMannequin::FindWeaponType(TSubclassOf<AWeapons> WeaponType)
{
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i] && Inventory[i]->IsA(WeaponType))
		{
			return Inventory[i];
		}
	}
	return NULL;
}


////////////////////////////////////////////////////////////////////////////////////
// Player Input

void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &AMannequin::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMannequin::MoveRight);
	InputComponent->BindAxis("LookUpRate", this, &AMannequin::LookUpRate);
	InputComponent->BindAxis("LookUp", this, &AMannequin::AddControllerPitchInput);
	InputComponent->BindAxis("TurnRate", this, &AMannequin::TurnRate);
	InputComponent->BindAxis("Turn", this, &AMannequin::AddControllerYawInput);

	InputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &AMannequin::ToggleCrouch);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
	InputComponent->BindAction("Fire", IE_Released, this, &AMannequin::ReleaseTrigger);
	InputComponent->BindAction("NextWeapon", IE_Pressed, this, &AMannequin::NextWeapon);
	InputComponent->BindAction("PrevWeapon", IE_Pressed, this, &AMannequin::PrevWeapon);
	InputComponent->BindAction("ReloadWeapon", IE_Pressed, this, &AMannequin::ReloadWeapon);
	InputComponent->BindAction("AimWeapon", IE_Pressed, this, &AMannequin::StartAiming);
	InputComponent->BindAction("AimWeapon", IE_Released, this, &AMannequin::StopAiming);
}

void AMannequin::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const bool bLimitRotation = (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling());
		const FRotator Rotation = bLimitRotation ? GetActorRotation() : Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		bIsMovingForward = true;
		UpdateTile();
	}
	else
	{
		bIsMovingForward = false;
	}
}

void AMannequin::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
		bIsMovingRight = true;
		UpdateTile();
	}
	else
	{
		bIsMovingRight = false;
	}
}

void AMannequin::LookUpRate(float Value)
{
	AddControllerPitchInput(Value * ConBaseLookRate * GetWorld()->GetDeltaSeconds());
}

void AMannequin::TurnRate(float Value)
{
	AddControllerYawInput(Value * ConBaseLookRate * GetWorld()->GetDeltaSeconds());
	FRotator NewYawRot = GetActorRotation();
	LeanRate = FMath::FindDeltaAngleDegrees(NewYawRot.Yaw, LastYawRot.Yaw);
	LastYawRot = NewYawRot;
}

void AMannequin::ToggleCrouch()
{
	if (bIsCrouching)
	{
		CrouchTimeline->Reverse();
		bIsCrouching = false;
	}
	else
	{
		CrouchTimeline->Play();
		bIsCrouching = true;
	}
}

void AMannequin::NextWeapon()
{
	if (CurrentWeapon == nullptr) { return; }

	const int32 CurrentWeaponIndex = Inventory.IndexOfByKey(CurrentWeapon);
	const int32 LastWeaponIndex = Inventory.IndexOfByKey(Inventory.Last());

	if (CurrentWeaponIndex != LastWeaponIndex)
	{
		EquipWeapon(Inventory[CurrentWeaponIndex + 1]);
		DisableAiming();
	}
}

void AMannequin::PrevWeapon()
{
	if (CurrentWeapon == nullptr) { return; }
	
	const int32 CurrentWeaponIndex = Inventory.IndexOfByKey(CurrentWeapon);
	const int32 FirstWeaponIndex = Inventory.IndexOfByKey(Inventory[0]);

	if (CurrentWeaponIndex != FirstWeaponIndex)
	{
		EquipWeapon(Inventory[CurrentWeaponIndex - 1]);
		DisableAiming();
	}
}

void AMannequin::PullTrigger()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->StartFire();
	}
}

void AMannequin::ReleaseTrigger()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->StopFire();
	}
}

void AMannequin::StartAiming()
{
	if (CurrentWeapon != nullptr && !GetIsBlocked())
	{
		if (CurrentWeapon->CanAim())
		{
			EnableAiming();
		}
		bWantsToAim = true;
	}
}

void AMannequin::StopAiming()
{
	if (CurrentWeapon != nullptr)
	{
		if (bIsAiming)
		{
			DisableAiming();
		}
		bWantsToAim = false;
	}
}

void AMannequin::ReloadWeapon()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->StartReload();
	}
}

void AMannequin::UpdateSensitivity(float AimSens_Mouse, float ADS_MouseSens, float AimSens_Controller, float ADS_ConSens)
{
	MouseSens_Modifier = AimSens_Mouse;
	MouseSens_ADS_Modifier = ADS_MouseSens;
	ConSens_Modifier = AimSens_Controller;
	ConSens_ADS_Modifier = ADS_ConSens;
}


////////////////////////////////////////////////////////////////////////////////////
// Weapon usage

void AMannequin::UpdateAim(bool bAim)
{
	bAim ? EnableAiming() : DisableAiming();
}

void AMannequin::ResetInputOnPause()
{
	StopAiming();
	ReleaseTrigger();
}

void AMannequin::EnableAiming()
{
	bIsAiming = true;
	float Zoom = CurrentWeapon->GetAimZoom();
	FMath::Clamp<float>(Zoom, 0.f, DefaultFOV);
	FirstPersonCameraComponent->SetFieldOfView(Zoom);
	FPMesh->SetVisibility(false);
	CurrentWeapon->SetWeaponVisibility(bIsAiming);
	EnableAimingEffect();

	MouseBaseLookRate = MouseSens_ADS_Modifier;
	ConBaseLookRate = ConSens_ADS_Modifier;
}

void AMannequin::DisableAiming()
{
	bIsAiming = false;
	FirstPersonCameraComponent->SetFieldOfView(DefaultFOV);
	FPMesh->SetVisibility(true);
	CurrentWeapon->SetWeaponVisibility(bIsAiming);
	DisableAimingEffect();

	MouseBaseLookRate = MouseSens_Modifier;
	ConBaseLookRate = ConSens_Modifier;
}


////////////////////////////////////////////////////////////////////////////////////
// Movement

void AMannequin::Landed(const FHitResult & Hit)
{
	if (IsPlayerControlled())
	{
		UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 1.f, this, 500.f);
	}
}

void AMannequin::SmoothCrouchTimeline(float Value)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp<float>(StandHeight, CrouchedEyeHeight, Value));

	// Prevent TPC falling through map when capsule component(root component) is shrunk to half the mesh size.
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, FMath::Lerp<float>(-StandHeight, -45.f, Value)));
}

void AMannequin::CrouchOnTimelineFinished()
{
	// No default behaviour
}

FRotator AMannequin::GetAimOffset() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

void AMannequin::IsWeaponBlocked()
{
	FHitResult Hit;
	const FVector StartTrace = MuzzleEndLocation->GetComponentLocation();
	const FVector ForwardVector = MuzzleEndLocation->GetForwardVector();
	const float WeaponLength = CurrentWeapon ? CurrentWeapon->GetExtraWeaponLength() : 50.f;
	const FVector EndTrace = StartTrace + (ForwardVector * WeaponLength);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(CurrentWeapon);
	
	if (GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartTrace,
		EndTrace,
		COLLISION_WEAPONCOLLISION,
		TraceParams)
		)
	{
		bIsBlocked = true;
		StopAiming();
		ReleaseTrigger();
	}
	else
	{
		bIsBlocked = false;
	}
}

void AMannequin::UpdateTile()
{
	if (GetCharacterMovement()->CurrentFloor.bBlockingHit)
	{
		FHitResult Hit = GetCharacterMovement()->CurrentFloor.HitResult;
		ATile * Tile = Cast<ATile>(Hit.GetActor());
		if (Tile && Tile != CurrentTile)
		{
			CurrentTile = Tile;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////
// Animation


USkeletalMeshComponent * AMannequin::GetAnimationMesh() const 
{
	USkeletalMeshComponent * UseMesh = IsPlayerControlled() ? FPMesh : GetMesh();
	return UseMesh;
}

float AMannequin::PlayAnimMontage(UAnimMontage * AnimMontage, float InPlayRate, FName StartSectionName)
{
	if (AnimMontage && GetAnimationMesh() && GetAnimationMesh()->AnimScriptInstance)
	{
		return GetAnimationMesh()->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}
	return 0.0f;
}

void AMannequin::StopAnimMontage(UAnimMontage * AnimMontage)
{
	if (AnimMontage && GetAnimationMesh() && GetAnimationMesh()->AnimScriptInstance &&
		GetAnimationMesh()->AnimScriptInstance->Montage_IsPlaying(AnimMontage))
	{
		GetAnimationMesh()->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOut.GetBlendTime(), AnimMontage);
	}
}

void AMannequin::StopAllAnimMontages()
{
	if (GetAnimationMesh() && GetAnimationMesh()->AnimScriptInstance)
	{
		GetAnimationMesh()->AnimScriptInstance->Montage_Stop(0.0f);
	}
}


////////////////////////////////////////////////////////////////////////////////////
// FX

void AMannequin::PlaySound(USoundBase * Sound)
{
	if (Sound != nullptr)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, Sound, GetActorLocation());
	}
}

void AMannequin::GetFootStepSound()
{
	if (GetCharacterMovement()->CurrentFloor.bBlockingHit)
	{
		FHitResult Hit = GetCharacterMovement()->CurrentFloor.HitResult;
		FTransform const SpawnTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint);
		AFootStepFX * Effect = GetWorld()->SpawnActorDeferred<AFootStepFX>(FootStepFX, SpawnTransform);

		if (Effect)
		{
			Effect->SurfaceHit = Hit;
			UGameplayStatics::FinishSpawningActor(Effect, SpawnTransform);
		}
		if (!bIsCrouching && IsPlayerControlled())
		{
			UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 1.f, this, 500.f);
		}
	}
}

