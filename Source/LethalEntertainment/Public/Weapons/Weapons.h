// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Weapons.generated.h"


class UAnimMontage;
class AMannequin;
class UAudioComponent;
class UParticleSystemComponent;
class UTimelineComponent;
class UForceFeedbackEffect;
class UCameraShake;
class USoundBase;

/** Determines state and what actions the weapon can perform */
UENUM()
enum class EWeaponState : uint8 {
	EIdle,
	EFiring,
	ECocking,
	EReloading,
	EEquipping
};

/** What weapon reloads (mag or single bullets) */
UENUM()
enum class EWeaponReloadType : uint8 {
	ESingle		UMETA(DisplayName = "Single"),
	EMag		UMETA(DisplayName = "Mag")
};

/** Basic data for weapon */
USTRUCT(BlueprintType)
struct FWeaponData {
	GENERATED_USTRUCT_BODY()

	/** True if weapon is fully automatic */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	bool bIsAuto;

	/** True if weapon needs cocked before each shot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	bool bNeedCocked;

	/** True if weapon has infinite supply of ammo */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	bool bInfiniteAmmo;

	/** Max ammo player can carry for weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (ClampMin = 0, ClampMax = 100))
	int32 MaxAmmo;

	/** Max ammo that can be in one magazine */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 50))
	int32 MaxMag;

	/** Time between each shot of the weapon (Fire rate) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (ClampMin = 0.1, ClampMax = 5))
	float TimeBetweenShots;

	/** Length of reload */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (ClampMin = 0.1, ClampMax = 1))
	float ReloadTime;

	/** Camera zoom when aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (ClampMin = 10, ClampMax = 100))
	float AimZoom;

	/** Upwards pitch from recoil during firing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (ClampMin = 0.01, ClampMax = 1))
	float RecoilPitch;

	/** Right yaw from recoil during firing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (ClampMin = 0, ClampMax = 0.1))
	float RecoilRight;

	/** Left yaw from recoil during firing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (ClampMin = -0.1, ClampMax = 0))
	float RecoilLeft;

	/** Length of weapon barrel, for checking collisions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (ClampMin = 10, ClampMax = 100))
	float ExtraWeaponLength;

	/** Name to be displayed on player UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	FString WeaponName;

	/** Crosshair to display on player ui for this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	UMaterialInstance * Crosshair;

	/** Display on player ui for this weapon when aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	TSubclassOf<UUserWidget> AimDisplay;

	/** Defaults */
	FWeaponData ()
	{
		bIsAuto = false;
		bNeedCocked = false;
		bInfiniteAmmo = false;
		MaxAmmo = 100;
		MaxMag = 50;
		TimeBetweenShots = 1.f;
		ReloadTime = 1.f;
		AimZoom = 50.f;
		ExtraWeaponLength = 50.f;
		RecoilPitch = 1.f;
		RecoilRight = .1f;
		RecoilLeft = .1f;
	}
};

/** Animations for each mesh */
USTRUCT()
struct FWeaponAnim {
	GENERATED_USTRUCT_BODY()

	/** Animations played for first person character (FPC) */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage * AnimFPC;

	/** Animations played for third person character (TPC) */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage * AnimTPC;
};


UCLASS()
class LETHALENTERTAINMENT_API AWeapons : public AActor {
	GENERATED_BODY()

protected:

	////////////////////////////////////////////////////////////////////////////////
	// Control

	/** Current weapon state */
	EWeaponState CurrentState = EWeaponState::EIdle;

	////////////////////////////////////////////////////////////////////////////////
	// Reading Data

	/** Owning Pawn */
	AMannequin * CharacterPawn;

	/** Location on gun fp mesh where projectiles/Particles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USceneComponent * MuzzleLocationFP;

	/** Location on gun tp mesh where projectiles/Particles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USceneComponent * MuzzleLocationTP;

	/** Struct variable which determines weapon behaviour, exposed to BP for player ui */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	FWeaponData WeaponData;

	/** Enum variable of weapon reload type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	EWeaponReloadType ReloadType;

	/** Current ammo remaining for weapon */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	/** Current ammo remaining in mag */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	int32 CurrentMag;

private:

	////////////////////////////////////////////////////////////////////////////////
	// Weapon

	/** Component for recoil timeline */
	UTimelineComponent * RecoilTimeline;

	/** Component for recoil recovery timeline */
	UTimelineComponent * RecoveryTimeline;

	/** Curve for recoil */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UCurveFloat * FRecoilCurve;

	/** Curve for recoil recovery */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UCurveFloat * FRecoilRecoveryCurve;


	////////////////////////////////////////////////////////////////////////////////
	// Control

	/** Has equip anim finished */
	bool bIsEquipped;

	/** Is eqip anim playing */
	bool bPendingEquip;

	/** Is reload anim playing */
	bool bPendingReload;

	/** Is weapon firing */
	bool bWantsToFire;

	/** Has cocked anim finished */
	bool bIsCocked;

	/** Is cocked anim playing */
	bool bCockingWeapon;

	/** Is weapon recoil still in effect */
	bool bHasRecoveredRecoil;

	/** Last time weapon was fired */
	float LastFireTime;

	/** Upwards recoil from firing */
	float RecoilPitch;

	/** Sideways recoil from firing */
	float RecoilYaw;

	/** Owners target pitch for recoil recovery */
	float InitialPitch;

	FRotator InitialRotation;

	/** Current rotation of owner pawn */
	float TargetPitch;

	/** Current owners yaw */
	float CurrentYaw;

	/** Current owners pitch */
	float CurrentPitch;

	/** Owners current total rotaion*/
	FRotator CurrentRot;

	/** Target total rotaion for owner */
	FRotator TargetRot;

	/** For efficient management of firing */
	FTimerHandle FireTimer;

	/** For efficient management of equipping */
	FTimerHandle EquipedTimer;

	/** For efficient management of reloading */
	FTimerHandle ReloadTimer;

	/** For efficient management of stop reloading */
	FTimerHandle StopReloadTimer;

	/** For efficient management of cocking*/
	FTimerHandle CockingTimer;

	////////////////////////////////////////////////////////////////////////////////
	// Reading Data

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent * FPMesh;

	/** Gun mesh: 3rd person view (seen by others) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent * TPMesh;

	/** UI display when aiming */
	UUserWidget * AimDisplay;


	////////////////////////////////////////////////////////////////////////////////
	// Animation

	/** Weapon fire anim */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FWeaponAnim FireAnim;

	/** Weapon reload anim */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FWeaponAnim ReloadAnim;

	/** Weapon start reload anim (Used for bullet reload weapons) */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FWeaponAnim StartReloadAnim;

	/** Weapon end reload anim (Used for bullet reload weapons) */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FWeaponAnim EndReloadAnim;

	/** Weapon equip anim */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FWeaponAnim EquipAnim;

	/** Weapon unequip anim */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FWeaponAnim UnEquipAnim;

	/** Weapon cocking anim */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FWeaponAnim CockAnim;

	
	////////////////////////////////////////////////////////////////////////////////
	// FX

	/** Controller vibration on firing */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UForceFeedbackEffect * FireForceFeedback;

	/** Camera shake on firing */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TSubclassOf<UCameraShake> FireCamShakeBP;

	/** Component for fx on fp mesh */
	UParticleSystemComponent * ParticleComp1;

	/** Component for fx on tp mesh */
	UParticleSystemComponent * ParticleComp2;

	/** Muzzle flash fx */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem * MuzzleFlash;

	/** Fire sound fx */
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase * FireSound;

	/** Unload sound fx */
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase * UnloadSound;

	/** Load sound fx */
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase * LoadSound;

	/** Cocking sound fx */
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase * CockingSound;

	/** Empty sound fx */
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase * EmptySound;

	/** Equip sound fx */
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase * EquipSound;

	/** Unequip sound fx */
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase * UnEquipSound;

public:

	/** Setup components & default values */
	AWeapons();


	////////////////////////////////////////////////////////////////////////////////
	// Input
	
	/** Start weapon fire */
	void StartFire();

	/** Stop weapon fire */
	void StopFire();

	/** Start Reloading weapon */
	void StartReload();


	////////////////////////////////////////////////////////////////////////////////
	// Inventory

	/** Weapon is being equipped by owner pawn */
	void OnEquip();

	/** Weapon is now equipped by owner pawn */
	void OnEquippedFinished();

	/** Weapon is being unequipped by owner pawn */
	void OnUnEquip();

	/** Set weapons owning pawn */
	void SetOwningPawn(AMannequin * NewOwner);

	/** Destroys fp mesh on owner death to avid being seen */
	void DestroyFPMesh();


	////////////////////////////////////////////////////////////////////////////////
	// Ammo

	/** Add ammo when picked up */
	void GiveAmmo(int32 AmountToAdd);

	/** Is ammo count less than max */
	bool NeedAmmo() const { return CurrentAmmo < WeaponData.MaxAmmo; }


	////////////////////////////////////////////////////////////////////////////////
	// Weapon Usage

	/** Stops weapon actions */
	void StopActions();

	/** Set when weapon is visible to player (when aiming) */
	void SetWeaponVisibility(bool bIsAiming);


	////////////////////////////////////////////////////////////////////////////////
	// Control

	/** Can aim weapon */
	bool CanAim() const;

	/** Can show weapon crosshair (when ready to fire) */
	bool ShowCrosshair() const { return CanFire(); }


	////////////////////////////////////////////////////////////////////////////////
	// Reading Data

	/** Get how much field of vision camera has when aiming */
	float GetAimZoom() const { return WeaponData.AimZoom; }

	/** Get length of weapon to check if blocked by environment */
	float GetExtraWeaponLength() const { return WeaponData.ExtraWeaponLength; }

	/** Get weapons dynamic crosshair */
	UMaterialInstance * GetCrosshair() const { return WeaponData.Crosshair; }

	/** Get weapon UI display when aiming */
	UUserWidget * GetAimDisplay() const { return AimDisplay; }
	
protected:

	virtual void BeginPlay() override;


	////////////////////////////////////////////////////////////////////////////////
	// Weapon Usage
	
	/** Weapon specific fire implementation */
	UFUNCTION()
	virtual void FireWeapon() { /* Default behaviour */ }

	/** Called when weapon state is no longer firing */
	UFUNCTION()
	virtual void OnFireFinished();

	/** Enable weapon aiming */
	void EnableAiming();

	/** Disable weapon aiming */
	void DisableAiming();


	////////////////////////////////////////////////////////////////////////////////
	// Control

	/** Determines what state weapon is in */
	void DetermineWeaponState();

	/** Can weapon be fired */
	bool CanFire() const;

	/** Can weapon be reloaded */
	bool CanReload() const;

	/** Can weapon be cocked */
	bool CanCockWeapon() const;

	
	////////////////////////////////////////////////////////////////////////////////
	// Reading Data

	/** Get direction weapon should be aiming */
	FVector AimDirction() const;

	/** Adjust aiming depending on controller */
	FVector GetAdjustedAim() const;

	/** Where to start weapon trace */
	FVector StartTraceVectorOnFire(const FVector & AimDir) const;

	FHitResult WeaponTrace(const FVector & TraceFrom, const FVector & TraceTo);

	FVector GetMuzzleLocation() const;


	////////////////////////////////////////////////////////////////////////////////
	// Animation

	/** Play weapon animation */
	float PlayAnimation(const FWeaponAnim & Anim);

	/** Stop weapon animation */
	void StopAnimation(const FWeaponAnim & Anim);


	////////////////////////////////////////////////////////////////////////////////
	// FX

	/** Play sounds on weapon */
	UFUNCTION()
	UAudioComponent * PlayWeaponSound(USoundBase * Sound);

	/** Play particle fx on weapon */
	void SpawnParticleEffect(UParticleSystem * Particle);
	
private:

	////////////////////////////////////////////////////////////////////////////////
	// Inventory

	/** Attaches selected weapon to character */
	void AttachMeshToCharacter();

	/** Detaches weapon from character */
	void DetachMeshFromCharacter();


	////////////////////////////////////////////////////////////////////////////////
	// Weapon Usage

	/** Fires weapon in time with fire rate */
	void PullTrigger();

	/** Called if weapon state is firing */
	void OnFire();

	/** Fire behaviour for players */
	void OnFirePlayerBehaviour();

	/** Delegate function to be binded with RecoilTimelineFunc */
	FOnTimelineFloat RecoilInterpFunction{};

	/** Delegate function to be binded with RecoveryTimelineFunc */
	FOnTimelineFloat RecoveryInterpFunction{};

	/** Delegate function to be binded with RecoilOnTimelineFinished */
	FOnTimelineEvent RecoilTimelineFinished{};

	/** Delegate function to be binded with RecoveryOnTimelineFinished */
	FOnTimelineEvent RecoveryTimelineFinished{};

	/** Gets value of float over timeline duration */
	UFUNCTION()
	void RecoilTimelineFunc(float Value);

	/** Gets value of float over timeline duration */
	UFUNCTION()
	void RecoveryTimelineFunc(float Value);

	/** Any behaviour when timeline finished */
	UFUNCTION()
	void RecoilOnTimelineFinished();

	/** Any behaviour when timeline finished */
	UFUNCTION()
	void RecoveryOnTimelineFinished();

	/** Get the owners movements while controlling recoil */
	void UpdateRecoveryRotation();

	void GetNewTargetRotation();

	/** Reload weapon depending on reload type */
	void ReloadWeapon();

	/** Reloads magazine into weapon */
	void ReloadMag();

	/** Reloads single bullets into weapon */
	void ReloadBullet();

	/** Stop reloading weapon */
	void OnReloadFinished();

	/** Stop while reloading */
	void CancelReload();

	/** Cocks weapon */
	void OnCockingWeapon();

	/** Weapon cocked */
	void OnCockingFinished();


	////////////////////////////////////////////////////////////////////////////////
	// Control

	/** Sets the current state of the weapon*/
	void SetWeaponState(EWeaponState State);

};
