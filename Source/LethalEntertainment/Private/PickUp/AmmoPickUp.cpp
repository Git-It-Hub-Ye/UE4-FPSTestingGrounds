// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Player/Mannequin.h"
#include "Weapons/Weapons.h"
#include "AmmoPickUp.h"


void AAmmoPickUp::OnPlayerEnterPickUpBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMannequin * Character = Cast<AMannequin>(OtherActor);

	if (Character && !Character->IsPlayerControlled()) { return; }

	AWeapons * Weapon = (Character ? Character->FindWeaponType(WeaponType) : NULL);
	
	if (Weapon && Weapon->NeedAmmo())
	{
		Weapon->GiveAmmo(AmmoAmount);
		OnPickUp();
	}
}

