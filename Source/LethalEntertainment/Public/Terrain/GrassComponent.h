// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LETHALENTERTAINMENT_API UGrassComponent : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	/** Dimensions of box to spawn grass inside */
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	FBox SpawnExtent;

	/** How many to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	int SpawnCount;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	/** Spawns all grass in scene */
	void SpawnGrass();

};
