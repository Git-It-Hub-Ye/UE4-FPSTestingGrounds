// Copyright 2018 - 2021 Stuart McDonald.

#include "LethalEntertainment.h"
#include "PickUps.h"


APickUps::APickUps()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickUpRoot"));
	SetRootComponent(PickUpRoot);

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PickUpMesh->SetupAttachment(PickUpRoot);

	PickUpBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickUpBox"));
	PickUpBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	PickUpBox->bGenerateOverlapEvents = true;
	PickUpBox->OnComponentBeginOverlap.AddDynamic(this, &APickUps::OnPlayerEnterPickUpBox);
	PickUpBox->SetupAttachment(PickUpRoot);
}

void APickUps::OnPlayerEnterPickUpBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Empty default method for all pickups.
}

void APickUps::OnPickUp()
{
	if (OnPickUpSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OnPickUpSound, GetActorLocation());
	}

	// Destroyed along with tile
	PickUpMesh->SetVisibility(false);
	PickUpBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

