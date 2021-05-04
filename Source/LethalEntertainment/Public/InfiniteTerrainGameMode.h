// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "InfiniteTerrainGameMode.generated.h"

class ATile;
class ULethalSaveGame;

UCLASS()
class LETHALENTERTAINMENT_API AInfiniteTerrainGameMode : public AGameModeBase
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

	/** Highest Number of tiles player has entered */
	int32 HighScore;

	/** Save slot name */
	FString SaveSlot1 = FString(TEXT("SaveLot1"));

	/** Save class variable */
	UPROPERTY()
	ULethalSaveGame * SaveGameRef;

	/** The class for save game */
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<ULethalSaveGame> SaveGameClass;

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
	void GameOver(APlayerController * PC);

	/** Get current player score */
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const { return Score; }

	/** Get current player score */
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetHighScore() const { return HighScore; }

protected:
	virtual void BeginPlay() override;

private:
	/** Adds each nav mesh instance to pool */
	void AddToPool(class ANavMeshBoundsVolume * VolumeToAdd);

	/** Gets save game class */
	void LoadGameData();

	/** Sets save game class */
	void SaveGameData();
	
};
