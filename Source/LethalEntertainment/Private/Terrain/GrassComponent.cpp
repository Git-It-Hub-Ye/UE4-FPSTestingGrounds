// Fill out your copyright notice in the Description page of Project Settings.

#include "LethalEntertainment.h"
#include "GrassComponent.h"

void UGrassComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnGrass();
}


void UGrassComponent::SpawnGrass()
{
	for (size_t i = 0; i < SpawnCount; i++) {
		FVector Location = FMath::RandPointInBox(SpawnExtent);
		AddInstance(FTransform(Location));
	}
}


void UGrassComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

