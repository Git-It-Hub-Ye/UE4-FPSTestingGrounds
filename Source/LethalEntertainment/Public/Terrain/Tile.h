// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.generated.h"


class UActorPool;
class UArrowComponent;

/** Positions for each instance spawned */
USTRUCT()
struct FSpawnPosition {
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

/** Data for spawned actors */
USTRUCT(BlueprintType)
struct FSpawnParameters {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Spawn")
	int MinSpawn = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Spawn")
	int MaxSpawn = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Spawn")
	int Radius = 500;

	UPROPERTY(BlueprintReadWrite, Category = "Spawn")
	float MinScale = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Spawn")
	float MaxScale = 1;
};

UCLASS()
class LETHALENTERTAINMENT_API ATile : public AActor
{
	GENERATED_BODY()
	
protected:
	/** Min extent of spawned actors */
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	FVector MinExtent;

	/** Max extent of spawned actors */
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	FVector MaxExtent;

	/** Moves nav mesh to position correctly on tile */
	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavMeshOffset;

private:
	/** Root component (So spawn location can be moved) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Defaults")
	USceneComponent * RootComp;

	/** Location where nect tile will spawn */
	UPROPERTY(VisibleDefaultsOnly, Category = "Defaults")
	UArrowComponent * SpawnLocation;

	/** Pool of nav mesh actors */
	UActorPool * Pool;

	/** Tile nav mesh */
	AActor * NavMesh;

	/** All spawned actors to be destroyed */
	TArray<AActor*> GarbageProps;

public:	
	ATile();

	/** Spawns actors at empty location given */
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, FSpawnParameters SpawnParameters);

	/** Spawns AI pawns at empty location given */
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, FSpawnParameters SpawnParameters);

	/** Sets reference from GameMode to ActorPool*/
	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool * InPool);

	/** Gets location to attach next tile */
	FTransform GetAttachLocation() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	/** Returns true if location is empty */
	bool FindEmptyLocation(FVector & OutLocation, int32 Radius);

	/** Generates random positions */
	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, FSpawnParameters SpawnParameters);

	/** Spawns a random number of actors in our Tile, at random empty locations */
	void PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition & SpawnPosition);

	/** Spawns a random number of AI pawns in our Tile, at random empty locations */
	void PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition & SpawnPosition);

	/** Checks if selected location is empty */
	bool CanSpawnAtLocation(FVector Location, float Radius);

	/** Positions nav mesh recieved from ActorPool onto spawned tile */
	void PositionNavMesh();
	
};
