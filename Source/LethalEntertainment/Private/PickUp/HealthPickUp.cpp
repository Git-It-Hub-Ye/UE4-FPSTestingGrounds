// Fill out your copyright notice in the Description page of Project Settings.

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

