// Copyright 2018 Stuart McDonald.

#pragma once

#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "Components/TimelineComponent.h"
#include "Mannequin.generated.h"

class AWeapons;
class AFootStepFX;
class ATile;

UENUM()
enum class ECharacterType : uint8 {
	EPlayer			UMETA(DisplayName = "Player"),
	EAIBot			UMETA(DisplayName = "AIBot"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMannequinDelegate);

UCLASS()
class LETHALENTERTAINMENT_API AMannequin : public ACharacter, public IAISightTargetInterface
{
	GENERATED_BODY()

protected:

	/////////////////////////////////////////////////////////////////////////////////////////
	// Weapon

	/** Is character aiming */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsAiming;


	/////////////////////////////////////////////////////////////////////////////////////////
	// Movement

	/** Is character crouching */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsCrouching;

	/** Is character blocked */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsBlocked;

private:

	/////////////////////////////////////////////////////////////////////////////////////////
	// Mesh & Camera

	/** First Person Camera */
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
	UCameraComponent * FirstPersonCameraComponent;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent * FPMesh;

	/** Who controls the character */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	ECharacterType CharacterType;

	/** Locations on character which can be seen by ai */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TArray<FName> BodySockets;

	/** Location from where weapon is gripped to end of muzzle (Used to detect collision) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USceneComponent * MuzzleEndLocation;

	/** Current tile character is on */
	ATile * CurrentTile;


	/////////////////////////////////////////////////////////////////////////////////////////
	// Damage

	/** Starting health of character */
	UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (ClampMin = 10, ClampMax = 100))
	int32 StartingHealth = 60;

	/** Characters current health status */
	int32 CurrentHealth;

	/** Has character been hit */
	bool bIsHit;

	/** Is character health at zero */
	bool bIsDead;

	/** Bone name that was hit */
	FName HitBoneName;

	/** Component for hit behaviour */
	UTimelineComponent * HitTimeline;

	/** Curve for hit reaction */
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	UCurveFloat * FHitReactionCurve;

	/** Shake to play when damaged */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TSubclassOf<UCameraShake> DamageCamShakeBP;


	/////////////////////////////////////////////////////////////////////////////////////////
	// Inventory

	/** Characters default weapons */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<TSubclassOf<AWeapons>> DefaultInventory;

	/** Characters weapon inventory */
	TArray<AWeapons *> Inventory;

	/** Current weapon that is equipped */
	AWeapons * CurrentWeapon;


	/////////////////////////////////////////////////////////////////////////////////////////
	// Weapon

	/** Characters default field of vision */
	float DefaultFOV;

	/** Is player holding aim input */
	bool bWantsToAim;


	/////////////////////////////////////////////////////////////////////////////////////////
	// Movement

	/** Is character moving on X-axis */
	bool bIsMovingForward;

	/** Is character moving on Y-axis */
	bool bIsMovingRight;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	float BaseTurnRate;

	/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	float BaseLookUpRate;

	/** Amount to modifiy by while crouching */
	float CrouchingSpeedModifier;

	/** Amount to modifiy by while aiming */
	float AimingSpeedModifier;

	/** Standing height of capsule */
	float StandHeight;

	/** Component for crouch behaviour */
	UTimelineComponent * CrouchTimeline;

	/** Curve for crouch */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UCurveFloat * FCrouchCurve;


	/////////////////////////////////////////////////////////////////////////////////////////
	// FX

	/** Spawn Sound */
	UPROPERTY(EditAnywhere, Category = "FX")
	class USoundBase * SpawnSound;

	/** Dead sound */
	UPROPERTY(EditAnywhere, Category = "FX")
	class USoundBase * PowerDownSound;

	/** FootStep FX for different surfaces */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TSubclassOf<AFootStepFX> FootStepFX;

public:

	/** Setup components & default values */
	AMannequin(const FObjectInitializer & ObjectInitializer);

	/** Update character (Is blocked) */
	virtual void Tick(float DeltaSeconds) override;


	/////////////////////////////////////////////////////////////////////////////////////////
	// Mesh & Camera

	/** Checks if sockets on player mesh can be seen by AI */
	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor = NULL) const override;

	/** Return who controls the character */
	ECharacterType GetCharacterType() const { return CharacterType; }

	/** Return 1st person mesh for weapon attachment */
	USkeletalMeshComponent * GetFPMesh() const { return FPMesh; }

	/** Get current tile player character is on */
	void UpdateTile();

	/** Return current tile character is on */
	ATile * GetCurrentTile() const { return CurrentTile; }

	/////////////////////////////////////////////////////////////////////////////////////////
	// Damaged

	/** Is current health less than max health */
	bool NeedHealth() { return CurrentHealth < StartingHealth; }

	/** Adds health from pickup */
	void AddHealth(int32 HealthToAdd);

	/** Returns current health as a percentage, between zero and one */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const { return (float)CurrentHealth / (float)StartingHealth; }

	/** Is character dead */
	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const { return bIsDead; }

	/** Broadcasts to character controllers */
	FMannequinDelegate OnCharacterDeath;


	/////////////////////////////////////////////////////////////////////////////////////////
	// Player Input

	/** Bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Attempt to fire weapon */
	void PullTrigger();

	/** Stop firing weapon */
	void ReleaseTrigger();


	/////////////////////////////////////////////////////////////////////////////////////////
	// Weapon

	/** Updates if character can aim (Called when performing actions)*/
	void UpdateAim(bool bAim);

	/** Is player holding aim input (Called after action is performed) */
	bool WantsToAim() const { return bWantsToAim; }

	/** Reset player input when paused */
	void ResetInputOnPause();

	/** Is character aiming */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool IsAiming() const { return bIsAiming; }

	/** Return characters current held weapon */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	AWeapons * GetWeapon() const { return CurrentWeapon; }

	/** Return weapon type for ammo pickups */
	AWeapons * FindWeaponType(TSubclassOf<AWeapons> WeaponType);


	/////////////////////////////////////////////////////////////////////////////////////////
	// Movement

	/** Returns character movement speed modifier while aiming */
	float GetAimingSpeedModifier() const { return AimingSpeedModifier; }

	/** Returns character movement speed modifier while crouching */
	float GetCrouchingSpeedModifier() const { return CrouchingSpeedModifier; }

	/** Is character crouching */
	bool IsCrouching() const { return bIsCrouching; }

	/** Return characters aim rotation */
	UFUNCTION(BlueprintPure, Category = "Animation")
	FRotator GetAimOffset() const;

	/** Is weapon is being blocked */
	bool GetIsBlocked() const { return bIsBlocked; }

	/** Is character moving in any direction */
	bool IsMoving() const { return bIsMovingForward || bIsMovingRight; }


	/////////////////////////////////////////////////////////////////////////////////////////
	// Animation

	/** Play anim montage */
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	/** Stop playing montage */
	virtual void StopAnimMontage(class UAnimMontage* AnimMontage) override;

	/** Stop playing all montages */
	void StopAllAnimMontages();


	/////////////////////////////////////////////////////////////////////////////////////////
	// FX

	/** Get sound for surface type character is on */
	UFUNCTION(BlueprintCallable, Category = "FX")
	void GetFootStepSound();

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;


	/////////////////////////////////////////////////////////////////////////////////////////
	// Blueprint Implementable Events

	/** Add aim effect in blueprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void EnableAimingEffect();

	/** Remove aim effect in blueprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void DisableAimingEffect();

	/** Triggers Damage post process effect in bleprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void CharacterDamaged();

private:

	/////////////////////////////////////////////////////////////////////////////////////////
	// Mesh

	/** Return mesh to use for animations */
	USkeletalMeshComponent * GetAnimationMesh() const;

	/////////////////////////////////////////////////////////////////////////////////////////
	// Damaged

	/** Check if character is dead and run damage behaviour */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	/** Play effects on character when damaged */
	void PlayDamagedEffects();

	/** Delegate function to be binded with HitReactionTimeline */
	FOnTimelineFloat HitInterpFunction{};

	/** Delegate function to be binded with ResetPhysicsOnTimelineFinished */
	FOnTimelineEvent HitTimelineFinished{};

	/** Set physics on character when hit */
	UFUNCTION()
	void HitReactionTimeline(float Value);

	/** Reset character physics when timeline finished */
	UFUNCTION()
	void ResetPhysicsOnTimelineFinished();

	/** Handles character components and behaviour after death */
	void OnDeath();

	/** Sets characters ragdoll physics on death */
	void SetRagdollPhysics();


	/////////////////////////////////////////////////////////////////////////////////////////
	// Inventory

	/** Spawn default weapons */
	void SpawnDefaultInventory();

	/** Equip current weapon */
	void EquipWeapon(class AWeapons * Weapon);


	/////////////////////////////////////////////////////////////////////////////////////////
	// Input

	/** Move character along X-axis */
	void MoveForward(float Value);

	/** Move character along Y-axis */
	void MoveRight(float Value);

	/** Change characters view pitch */
	void LookUpRate(float Value);

	/** Turn character */
	void TurnRate(float Value);

	/** Make character crouch or stand */
	void ToggleCrouch();

	/** Get next weapon in inventory */
	void NextWeapon();

	/** Get previous weapon in inventory */
	void PrevWeapon();

	/** Attempt to aim weapon */
	void StartAiming();

	/** Stop aiming weapon */
	void StopAiming();

	/** Attemt to reload weapon */
	void ReloadWeapon();


	/////////////////////////////////////////////////////////////////////////////////////////
	// Weapon

	/** Aim weapon if character is able */
	void EnableAiming();

	/** Stop aiming weapon */
	void DisableAiming();


	/////////////////////////////////////////////////////////////////////////////////////////
	// Movement

	virtual void Landed(const FHitResult & Hit) override;

	/** Delegate function to be binded with SmoothCrouchTimeline */
	FOnTimelineFloat CrouchInterpFunction{};

	/** Delegate function to be binded with CrouchOnTimelineFinished */
	FOnTimelineEvent CrouchTimelineFinished{};

	/** Crouch '1st person' mesh smoothly */
	UFUNCTION()
	void SmoothCrouchTimeline(float Value);

	/** Any behaviour when timeline finished */
	UFUNCTION()
	void CrouchOnTimelineFinished();

	/** Check if weapon is blocked */
	void IsWeaponBlocked();


	/////////////////////////////////////////////////////////////////////////////////////////
	// FX

	/** Play sounds on character */
	void PlaySound(USoundBase * Sound);


};
