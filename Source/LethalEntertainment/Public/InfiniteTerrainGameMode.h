// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LethalEntertainmentGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

class ATile;

UCLASS()
class LETHALENTERTAINMENT_API AInfiniteTerrainGameMode : public ALethalEntertainmentGameMode
{
	GENERATED_BODY()
	
protected:
	/** To add nav mesh to array */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	class UActorPool * NavMeshPool;

private:
	/** Tiles to spawn on begin play */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (ClampMin = 1, ClampMax = 5))
	int32 InitialTilesToSpawn;

	/** Location for first tile to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FTransform SpawnTileTransform;

	/** Tile to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<ATile> TileBP;

	/** Number of tiles player has entered */
	int32 Score;

public:
	AInfiniteTerrainGameMode();

	/** Gets each nav mesh instance */
	void PopulateBoundsVolumePool();

	/** Spawns tile */
	UFUNCTION(BlueprintCallable, Category = "Tile")
	void SpawnTile();

	/** Add a point to score when a new tile is entered */
	UFUNCTION(BlueprintCallable, Category = "Tile")
	void PlayerConqueredTile();

	/** Performs game over behaviour */
	void GameOver(APawn * PlayerPawn);

	UFUNCTION(BlueprintPure, Category = "Game")
	int32 GetScore() const { return Score; }

protected:
	virtual void BeginPlay() override;

private:
	/** Adds each nav mesh instance to pool */
	void AddToPool(class ANavMeshBoundsVolume * VolumeToAdd);
	
};
