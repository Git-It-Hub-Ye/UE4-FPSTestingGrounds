// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "PickUp/PickUps.h"
#include "HealthPickUp.generated.h"

UCLASS()
class LETHALENTERTAINMENT_API AHealthPickUp : public APickUps
{
	GENERATED_BODY()

private:
	/** Health amount */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	int32 HealthAmount = 10;

protected:
	/** Triggered when player ovelaps pickup */
	UFUNCTION()
	virtual void OnPlayerEnterPickUpBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

};
