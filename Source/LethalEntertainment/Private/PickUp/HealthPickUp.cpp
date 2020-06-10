// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Player/Mannequin.h"
#include "HealthPickUp.h"


void AHealthPickUp::OnPlayerEnterPickUpBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMannequin * Character = Cast<AMannequin>(OtherActor);

	if (Character && !Character->IsPlayerControlled()) { return; }

	if (Character->NeedHealth())
	{
		Character->AddHealth(HealthAmount);
		OnPickUp();
	}
}

