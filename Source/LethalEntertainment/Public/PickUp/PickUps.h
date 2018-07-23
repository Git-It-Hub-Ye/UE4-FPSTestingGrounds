// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PickUps.generated.h"

UCLASS()
class LETHALENTERTAINMENT_API APickUps : public AActor
{
	GENERATED_BODY()

protected:
	/** On pickup sound FX */
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class USoundBase * OnPickUpSound;

private:
	/** Pickup mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent * PickUpMesh;

	/** Pickup root */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USceneComponent * PickUpRoot;

	/** Box trigger for ovelap events */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	UShapeComponent * PickUpBox;

public:
	APickUps();

protected:
	/** Triggered when player ovelaps pickup */
	UFUNCTION()
	virtual void OnPlayerEnterPickUpBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** Functionality to perform when picked up */
	UFUNCTION()
	virtual void OnPickUp();

};
