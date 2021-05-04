// Copyright 2018 - 2021 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Player/Mannequin.h"
#include "AIBot/CharacterAIController.h"
#include "Player/CharacterPlayerController.h"
#include "Perception/AISense_Hearing.h"
#include "Blueprint/UserWidget.h"
#include "LethalEntertainmentHUD.h"
#include "Weapons.h"


AWeapons::AWeapons()
{
	PrimaryActorTick.bCanEverTick = false;

	FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPMesh"));
	SetRootComponent(FPMesh);
	FPMesh->bCastDynamicShadow = false;
	FPMesh->CastShadow = false;
	FPMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TPMesh"));
	TPMesh->SetupAttachment(FPMesh);
	TPMesh->bCastDynamicShadow = false;
	TPMesh->CastShadow = true;
	TPMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MuzzleLocationFP = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocationFP"));
	MuzzleLocationFP->SetupAttachment(RootComponent);
	MuzzleLocationFP->SetRelativeLocation(FVector(0.f, 55.f, 11.f));
	MuzzleLocationFP->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	MuzzleLocationTP = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocationTP"));
	MuzzleLocationTP->SetupAttachment(TPMesh);
	MuzzleLocationTP->SetRelativeLocation(FVector(0.f, 55.f, 11.f));
	MuzzleLocationTP->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	RecoilTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimeLineRecoil"));
	RecoilInterpFunction.BindUFunction(this, FName("RecoilTimelineFunc"));
	RecoilTimelineFinished.BindUFunction(this, FName("RecoilOnTimelineFinished"));

	RecoveryTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimeLineRecovery"));
	RecoveryInterpFunction.BindUFunction(this, FName("RecoveryTimelineFunc"));
	RecoveryTimelineFinished.BindUFunction(this, FName("RecoveryOnTimelineFinished"));

	bIsEquipped = false;
	bIsCocked = false;

	bPendingEquip = false;
	bPendingReload = false;
	bCockingWeapon = false;

	CurrentAmmo = 0;
	CurrentMag = 0;
	LastFireTime = 0.f;

	bWantsToFire = false;
	bHasRecoveredRecoil = true;

	CurrentState = EWeaponState::EIdle;
}

void AWeapons::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = WeaponData.MaxAmmo;
	CurrentMag = WeaponData.MaxMag;

	AimDisplay = WeaponData.AimDisplay.GetDefaultObject();

	bIsCocked = WeaponData.bNeedCocked ? false : true;
	
	if (RecoilTimeline && FRecoilCurve)
	{
		RecoilTimeline->AddInterpFloat(FRecoilCurve, RecoilInterpFunction, FName("Alpha"));
		RecoilTimeline->SetTimelineFinishedFunc(RecoilTimelineFinished);
		RecoilTimeline->SetLooping(false);
	}
	if (RecoveryTimeline && FRecoilRecoveryCurve)
	{
		RecoveryTimeline->AddInterpFloat(FRecoilRecoveryCurve, RecoveryInterpFunction, FName("Alpha"));
		RecoveryTimeline->SetTimelineFinishedFunc(RecoveryTimelineFinished);
		RecoveryTimeline->SetLooping(false);
	}
}


////////////////////////////////////////////////////////////////////////////////
// Inventory

void AWeapons::OnEquip()
{
	bPendingEquip = true;

	AttachMeshToCharacter();

	DetermineWeaponState();

	float Duration = PlayAnimation(EquipAnim);
	if (Duration <= 0.f)
	{
		Duration = 0.5f;
	}

	PlayWeaponSound(EquipSound);

	GetWorldTimerManager().SetTimer(EquipedTimer, this, &AWeapons::OnEquippedFinished, Duration, false);
}

void AWeapons::OnEquippedFinished()
{
	GetWorldTimerManager().ClearTimer(EquipedTimer);
	bIsEquipped = true;
	bPendingEquip = false;

	DetermineWeaponState();

	if (CurrentMag <= 0 && CanReload())
	{
		StartReload();
	} 
	else if (!bIsCocked && CanCockWeapon())
	{
		OnCockingWeapon();
	}
	EnableAiming();
}

void AWeapons::OnUnEquip()
{
	DetachMeshFromCharacter();
	bIsEquipped = false;
	StopActions();
	DetermineWeaponState();
}

void AWeapons::SetOwningPawn(AMannequin * NewOwner)
{
	if (CharacterPawn != NewOwner)
	{
		Instigator = NewOwner;
		CharacterPawn = NewOwner;
		SetOwner(NewOwner);
	}
}

void AWeapons::AttachMeshToCharacter()
{
	DetachMeshFromCharacter();

	if (CharacterPawn)
	{
		USkeletalMeshComponent * TPCharacter = CharacterPawn->GetMesh();

		if (CharacterPawn->IsPlayerControlled())
		{
			USkeletalMeshComponent * FPCharacter = CharacterPawn->GetFPMesh();
			FPMesh->AttachToComponent(FPCharacter, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
			FPMesh->SetOwnerNoSee(false);
			FPMesh->SetHiddenInGame(false);
			
			TPMesh->AttachToComponent(TPCharacter, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint_1"));
			TPMesh->SetOwnerNoSee(true);
		}
		else
		{
			TPMesh->AttachToComponent(TPCharacter, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint_1"));
			TPMesh->SetOwnerNoSee(false);
		}

		TPMesh->SetHiddenInGame(false);
	}
}

void AWeapons::DetachMeshFromCharacter()
{
	FPMesh->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	FPMesh->SetHiddenInGame(true);

	TPMesh->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	TPMesh->SetHiddenInGame(true);
}

void AWeapons::DestroyFPMesh()
{
	FPMesh->DestroyComponent();
}


////////////////////////////////////////////////////////////////////////////////
// Input

void AWeapons::StartFire()
{
	if (CanFire() && !bWantsToFire)
	{
		bWantsToFire = true;
		UpdateRecoveryRotation();
		DetermineWeaponState();
	}
	else if (bPendingReload && CurrentMag > 0 && CurrentMag < WeaponData.MaxMag && CurrentAmmo > 0)
	{
		CancelReload();
	}
	else if (CurrentMag <= 0 && !bPendingReload)
	{
		PlayWeaponSound(EmptySound);
	}
}

void AWeapons::StopFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		DetermineWeaponState();
	}
}

void AWeapons::StartReload()
{
	if (CanReload())
	{
		if (bWantsToFire)
		{
			StopFire();
		}
		GetWorldTimerManager().ClearAllTimersForObject(this);
		bPendingReload = true;
		bCockingWeapon = false;
		DetermineWeaponState();
		DisableAiming();
		ReloadWeapon();
	}
}


////////////////////////////////////////////////////////////////////////////////
// Weapon Usage

void AWeapons::PullTrigger()
{
	// Firing, delayed for time between shots
	const float GameTime = GetWorld()->GetTimeSeconds();

	// Only full auto weapons can hold down trigger
	if (WeaponData.bIsAuto && LastFireTime > 0.f && WeaponData.TimeBetweenShots > 0.f && LastFireTime + WeaponData.TimeBetweenShots > GameTime)
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &AWeapons::OnFire, LastFireTime + WeaponData.TimeBetweenShots - GameTime, false);
	}
	else
	{
		OnFire();
	}
}

void AWeapons::OnFire()
{
	if (CanFire())
	{
		FireWeapon();

		float Duration = PlayAnimation(FireAnim);
		if (Duration <= 0.f)
		{
			Duration = 0.3f;
		}

		PlayWeaponSound(FireSound);
		SpawnParticleEffect(MuzzleFlash);
		OnFirePlayerBehaviour();

		CurrentMag--;
		LastFireTime = GetWorld()->GetTimeSeconds();
		WeaponData.bNeedCocked ? bIsCocked = false : bIsCocked = true;
	
		// Any actions to be taken after weapon fired?

		if (CurrentMag <= 0 && CanReload())
		{
			StopFire();
			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AWeapons::StartReload, Duration, false);
		}
		if (!bIsCocked && CanCockWeapon())
		{
			StopFire();
			GetWorld()->GetTimerManager().SetTimer(CockingTimer, this, &AWeapons::OnCockingWeapon, Duration, false);
		}
		if (WeaponData.bIsAuto)
		{
			GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AWeapons::PullTrigger, WeaponData.TimeBetweenShots, false);
		}
	}
}

void AWeapons::OnFirePlayerBehaviour()
{
	ACharacterPlayerController * PC = CharacterPawn ? Cast<ACharacterPlayerController>(CharacterPawn->GetController()) : nullptr;
	if (PC)
	{
		PC->ClientPlayCameraShake(FireCamShakeBP);

		if (FireForceFeedback && PC->IsVibrationEnabled())
		{
			PC->ClientPlayForceFeedback(FireForceFeedback, false, false, "Weapon");
		}

		bHasRecoveredRecoil = false;
		RecoilPitch = FMath::RandRange(WeaponData.RecoilPitch * 0.5, WeaponData.RecoilPitch) * -1;
		RecoilYaw = FMath::RandRange(WeaponData.RecoilLeft, WeaponData.RecoilRight);
		RecoveryTimeline->Stop();
		RecoilTimeline->PlayFromStart();

		UAISense_Hearing::ReportNoiseEvent(this, Instigator->GetActorLocation(), 1.f, Instigator, 3000.f);
	}
}

void AWeapons::OnFireFinished()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void AWeapons::UpdateRecoveryRotation()
{
	if (CharacterPawn && CharacterPawn->IsPlayerControlled())
	{
		if (bHasRecoveredRecoil)
		{
			InitialPitch = CharacterPawn->GetAimOffset().Pitch;
		}
	}
}

void AWeapons::RecoilTimelineFunc(float Value)
{
	if (CharacterPawn)
	{
		CharacterPawn->AddControllerPitchInput(FMath::Lerp<float>(RecoilPitch, 0.f, Value));
		CharacterPawn->AddControllerYawInput(FMath::Lerp<float>(RecoilYaw, 0.f, Value));
		GetNewTargetRotation();
	}
}

void AWeapons::RecoilOnTimelineFinished()
{
	RecoveryTimeline->PlayFromStart();
}

void AWeapons::RecoveryTimelineFunc(float Value)
{
	if (CharacterPawn)
	{
		GetNewTargetRotation();
		CharacterPawn->GetController()->SetControlRotation(FMath::Lerp(CurrentRot, TargetRot, Value));
	}
}

void AWeapons::RecoveryOnTimelineFinished()
{
	bHasRecoveredRecoil = true;
}

void AWeapons::GetNewTargetRotation()
{
	ACharacterPlayerController * PC = CharacterPawn ? Cast<ACharacterPlayerController>(CharacterPawn->GetController()) : nullptr;

	if (PC)
	{
		CurrentYaw = CharacterPawn->GetController()->GetControlRotation().Yaw;
		CurrentPitch = CharacterPawn->GetAimOffset().Pitch;

		TargetPitch = PC->CalculateTargetRotaion(InitialPitch, CharacterPawn->GetAimOffset().Pitch);
		InitialPitch = TargetPitch;

		// Stop recoil recovery when target reached
		if (FMath::IsNearlyEqual(CurrentPitch, TargetPitch, 0.1f) || CurrentPitch < TargetPitch)
		{
			RecoveryTimeline->Stop();
			RecoveryOnTimelineFinished();
		}

		CurrentRot = FRotator(CurrentPitch, CurrentYaw, 0.f);
		TargetRot = FRotator(TargetPitch, CurrentYaw, 0.f);
	}
}

void AWeapons::ReloadWeapon()
{
	WeaponData.bNeedCocked ? bIsCocked = false : bIsCocked = true;

	if (ReloadType == EWeaponReloadType::EMag)
	{
		float Duration = PlayAnimation(ReloadAnim) - 0.01f;
		if (Duration <= 0.f)
		{
			Duration = WeaponData.ReloadTime;
		}

		GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeapons::ReloadMag, Duration, false);
	}
	else if (ReloadType == EWeaponReloadType::ESingle)
	{
		float Duration = PlayAnimation(StartReloadAnim) - 0.01f;
		if (Duration <= 0.f)
		{
			Duration = WeaponData.ReloadTime;
		}

		GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeapons::ReloadBullet, Duration, false);

		PlayWeaponSound(UnloadSound);
	}
	PlayWeaponSound(LoadSound);
}

void AWeapons::ReloadMag()
{
	int32 RoundsDifference = WeaponData.MaxMag - CurrentMag;

	if (!WeaponData.bInfiniteAmmo)
	{
		if (CurrentAmmo < WeaponData.MaxAmmo)
		{
			CurrentMag += CurrentAmmo;
		}
		else if (CurrentMag < WeaponData.MaxMag)
		{
			CurrentMag += RoundsDifference;
		}

		CurrentAmmo -= RoundsDifference;
	}
	else if (WeaponData.bInfiniteAmmo)
	{
		CurrentMag += RoundsDifference;
	}

	CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, WeaponData.MaxMag);
	OnReloadFinished();
}

void AWeapons::ReloadBullet()
{
	float Duration = 0.f;

	if (CurrentMag >= WeaponData.MaxMag || CurrentAmmo <= 0)
	{
		Duration = PlayAnimation(EndReloadAnim);
		if (Duration <= 0.f)
		{
			Duration = 0.5f;
		}

		GetWorldTimerManager().SetTimer(StopReloadTimer, this, &AWeapons::OnReloadFinished, Duration, false);
	}
	else
	{
		Duration = PlayAnimation(ReloadAnim);
		if (Duration <= 0.f)
		{
			Duration = WeaponData.ReloadTime;
		}

		GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeapons::ReloadBullet, Duration, false);

		PlayWeaponSound(LoadSound);
		CurrentMag++;
		CurrentAmmo--;
	}
}

void AWeapons::OnReloadFinished()
{
	if (CurrentState == EWeaponState::EReloading)
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
		bPendingReload = false;
		bIsCocked = true;

		DetermineWeaponState();
		EnableAiming();
	}
}

void AWeapons::CancelReload()
{
	if (CurrentState == EWeaponState::EReloading)
	{
		StopAnimation(ReloadAnim);
		StopAnimation(StartReloadAnim);

		GetWorldTimerManager().ClearAllTimersForObject(this);

		bPendingReload = false;
		WeaponData.bNeedCocked ? bCockingWeapon = true : bCockingWeapon = false;
		DetermineWeaponState();

		if (CurrentState == EWeaponState::ECocking && CanCockWeapon())
		{
			float Duration = PlayAnimation(EndReloadAnim);
			if (Duration <= 0.f)
			{
				Duration = 0.5f;
			}
			GetWorldTimerManager().SetTimer(CockingTimer, this, &AWeapons::OnCockingFinished, Duration, false);
		}
	}
}

void AWeapons::OnCockingWeapon()
{
	bCockingWeapon = true;

	DetermineWeaponState();
	DisableAiming();

	float Duration = PlayAnimation(CockAnim);
	if (Duration <= 0.f)
	{
		Duration = 0.5f;
	}
	PlayWeaponSound(CockingSound);
	GetWorld()->GetTimerManager().SetTimer(CockingTimer, this, &AWeapons::OnCockingFinished, Duration, false);
}

void AWeapons::OnCockingFinished()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	bCockingWeapon = false;
	bIsCocked = true;
	DetermineWeaponState();
	EnableAiming();
}

void AWeapons::EnableAiming()
{
	if (CharacterPawn && CharacterPawn->IsPlayerControlled() && CharacterPawn->WantsToAim())
	{
		CharacterPawn->UpdateAim(CanAim());
	}
}

void AWeapons::DisableAiming()
{
	if (CharacterPawn && CharacterPawn->IsPlayerControlled() && CharacterPawn->IsAiming())
	{
		CharacterPawn->UpdateAim(CanAim());
	}
}

void AWeapons::StopActions()
{
	DisableAiming();
	RecoilTimeline->Stop();
	RecoveryTimeline->Stop();
	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (bWantsToFire)
	{
		StopFire();
	}
	if (bPendingReload)
	{
		StopAnimation(ReloadAnim);
		bPendingReload = false;
	}
	if (bCockingWeapon)
	{
		StopAnimation(CockAnim);
		bCockingWeapon = false;
	}
	if (bPendingEquip)
	{
		StopAnimation(EquipAnim);
		bPendingEquip = false;
	}
}

void AWeapons::GiveAmmo(int32 AmountToAdd)
{
	int32 AmmoUsed = FMath::Max(0, WeaponData.MaxAmmo - CurrentAmmo);

	AmountToAdd = FMath::Min(AmountToAdd, AmmoUsed);

	CurrentAmmo += AmountToAdd;

	if (CurrentMag <= 0 && CanReload())
	{
		StartReload();
	}
}


////////////////////////////////////////////////////////////////////////////////
// Control

void AWeapons::DetermineWeaponState()
{
	EWeaponState WeaponState = EWeaponState::EIdle;

	if (bIsEquipped)
	{
		if (bPendingReload && CanReload())
		{
			WeaponState = EWeaponState::EReloading;
		}
		else if (bCockingWeapon && CanCockWeapon())
		{
			WeaponState = EWeaponState::ECocking;
		}
		else if (bWantsToFire && CanFire())
		{
			WeaponState = EWeaponState::EFiring;
		}
	}
	else if (bPendingEquip)
	{
		WeaponState = EWeaponState::EEquipping;
	}

	SetWeaponState(WeaponState);
}

void AWeapons::SetWeaponState(EWeaponState State)
{
	const EWeaponState PrevState = CurrentState;

	if (PrevState == EWeaponState::EFiring && State != EWeaponState::EFiring)
	{
		OnFireFinished();
	}

	CurrentState = State;

	if (PrevState != EWeaponState::EFiring && State == EWeaponState::EFiring)
	{
		PullTrigger();
	}
}

void AWeapons::SetWeaponVisibility(bool bIsAiming)
{
	bIsAiming ? FPMesh->SetVisibility(false) : FPMesh->SetVisibility(true);
}

FVector AWeapons::AimDirction() const
{
	ACharacterPlayerController * const PlayerController = Instigator ? Cast<ACharacterPlayerController>(Instigator->Controller) : nullptr;
	FVector FinalAim = FVector::ZeroVector;

	if (PlayerController)
	{
		FVector CamLoc;
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
		FinalAim = CamRot.Vector();
	}
	else if (Instigator)
	{
		FinalAim = Instigator->GetBaseAimRotation().Vector();
	}

	return FinalAim;
}

FVector AWeapons::GetAdjustedAim() const
{
	ACharacterPlayerController * const PlayerController = Instigator ? Cast<ACharacterPlayerController>(Instigator->Controller) : nullptr;
	FVector FinalAim = FVector::ZeroVector;
	// If we have a player controller use it for the aim
	if (PlayerController)
	{
		FVector CamLoc;
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
		FinalAim = CamRot.Vector();
	}
	else if (Instigator)
	{
		// Now see if we have an AI controller - we will want to get the aim from there if we do
		ACharacterAIController * AIController = CharacterPawn ? Cast<ACharacterAIController>(CharacterPawn->Controller) : nullptr;
		if (AIController != nullptr)
		{
			FinalAim = AIController->GetControlRotation().Vector();
		}
		else
		{
			FinalAim = Instigator->GetBaseAimRotation().Vector();
		}
	}

	return FinalAim;
}

FVector AWeapons::StartTraceVectorOnFire(const FVector & AimDir) const
{
	ACharacterPlayerController * PC = CharacterPawn ? Cast<ACharacterPlayerController>(CharacterPawn->GetController()) : nullptr;
	ACharacterAIController * AIC = CharacterPawn ? Cast<ACharacterAIController>(CharacterPawn->GetController()) : nullptr;
	FVector StartTrace = FVector::ZeroVector;

	if (PC)
	{
		FRotator Rot;
		PC->GetPlayerViewPoint(StartTrace, Rot);

		StartTrace = StartTrace + AimDir * ((Instigator->GetActorLocation() - StartTrace | AimDir));
	}
	else if (AIC)
	{
		StartTrace = GetMuzzleLocation();
	}

	return StartTrace;
}

FHitResult AWeapons::WeaponTrace(const FVector & TraceFrom, const FVector & TraceTo)
{
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(Instigator);
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(
		Hit,
		TraceFrom,
		TraceTo,
		COLLISION_WEAPON,
		TraceParams
	);

	return Hit;
}

FVector AWeapons::GetMuzzleLocation() const
{
	return CharacterPawn && CharacterPawn->IsPlayerControlled() ? MuzzleLocationFP->GetSocketLocation("MuzzleLocationFP") : MuzzleLocationTP->GetSocketLocation("MuzzleLocationTP");
}

bool AWeapons::CanReload() const
{
	bool bMagLessThanMax = CurrentMag < WeaponData.MaxMag;
	bool bHasAmmo = CurrentAmmo > 0 || WeaponData.bInfiniteAmmo;
	bool bStateOkToReload = bIsEquipped && CurrentState != EWeaponState::EReloading;
	return bMagLessThanMax && bHasAmmo && bStateOkToReload;
}

bool AWeapons::CanCockWeapon() const
{
	bool bReady = WeaponData.bNeedCocked && !bIsCocked && CurrentMag > 0;
	return bReady;
}

bool AWeapons::CanFire() const
{
	bool bCanFire = CurrentMag > 0 && bIsCocked && CurrentState == EWeaponState::EIdle || CurrentState == EWeaponState::EFiring;
	bool bIsReady = CharacterPawn && !CharacterPawn->GetIsBlocked();
	return bCanFire && bIsReady;
}

bool AWeapons::CanAim() const
{
	bool bReady = bIsEquipped;
	bool bStateOkToAim = CurrentState == EWeaponState::EIdle || CurrentState == EWeaponState::EFiring;
	return bReady && bStateOkToAim;
}


////////////////////////////////////////////////////////////////////////////////
// FX

float AWeapons::PlayAnimation(const FWeaponAnim & Anim)
{
	float Duration = 0.f;

	if (CharacterPawn)
	{
		UAnimMontage * UseAnim = CharacterPawn->IsPlayerControlled() ? Anim.AnimFPC : Anim.AnimTPC;

		if (UseAnim)
		{
			Duration = CharacterPawn->PlayAnimMontage(UseAnim);
		}
	}

	return Duration;
}

void AWeapons::StopAnimation(const FWeaponAnim & Anim)
{
	if (CharacterPawn)
	{
		UAnimMontage * UseAnim = CharacterPawn->IsPlayerControlled() ? Anim.AnimFPC : Anim.AnimTPC;
		if (UseAnim)
		{
			CharacterPawn->StopAnimMontage(UseAnim);
		}
	}
}

UAudioComponent * AWeapons::PlayWeaponSound(USoundBase * Sound)
{
	UAudioComponent * AC = nullptr;

	if (Sound && CharacterPawn)
	{
		AC = UGameplayStatics::SpawnSoundAttached(Sound, CharacterPawn->GetRootComponent());
	}

	return AC;
}

void AWeapons::SpawnParticleEffect(UParticleSystem * Particle)
{
	if (Particle != nullptr)
	{
		if (CharacterPawn && CharacterPawn->IsPlayerControlled() && !CharacterPawn->IsAiming())
		{
			ParticleComp1 = UGameplayStatics::SpawnEmitterAttached(Particle, MuzzleLocationFP);
			ParticleComp1->bOnlyOwnerSee = true;
		}
		ParticleComp2 = UGameplayStatics::SpawnEmitterAttached(Particle, MuzzleLocationTP);
		ParticleComp2->bOwnerNoSee = true;
	}
}

