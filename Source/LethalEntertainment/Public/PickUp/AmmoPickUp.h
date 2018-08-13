// Copyright 2018 Stuart McDonald.

#pragma once

#include "PickUp/PickUps.h"
#include "AmmoPickUp.generated.h"

class AWeapons;

UCLASS()
class LETHALENTERTAINMENT_API AAmmoPickUp : public APickUps
{
	GENERATED_BODY()

protected:
	/** Weapon compatible with ammo */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<AWeapons> WeaponType;

private:
	/** Ammo amount */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	int32 AmmoAmount = 0;

protected:
	/** Triggered when player ovelaps pickup */
	UFUNCTION()
	virtual void OnPlayerEnterPickUpBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

};
