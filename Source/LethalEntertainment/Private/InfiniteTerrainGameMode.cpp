// Fill out your copyright notice in the Description page of Project Settings.

#include "LethalEntertainment.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"
#include "Terrain/Tile.h"
#include "Player/CharacterPlayerController.h"
#include "LethalEntertainmentHUD.h"
#include "InfiniteTerrainGameMode.h"


AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	NavMeshPool = CreateDefaultSubobject<UActorPool>(FName("NavMeshPool"));

	InitialTilesToSpawn = 1;
	SpawnTileTransform.SetLocation(FVector(0.f, 0.f, 0.f));
	Score = 0;
}

void AInfiniteTerrainGameMode::BeginPlay()
{
	PopulateBoundsVolumePool();
	for (int32 i = 0; i < InitialTilesToSpawn; i++)
	{
		SpawnTile();
	}
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	auto VolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());

	while (VolumeIterator) {
		AddToPool(*VolumeIterator);
		++VolumeIterator;
	}
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume * VolumeToAdd)
{
	NavMeshPool->Add(VolumeToAdd);
}

void AInfiniteTerrainGameMode::SpawnTile()
{
	ATile * TileToSpawn = Cast<ATile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, TileBP, SpawnTileTransform));

	if (TileToSpawn)
	{
		UGameplayStatics::FinishSpawningActor(TileToSpawn, SpawnTileTransform);
		TileToSpawn->SetPool(NavMeshPool);
		SpawnTileTransform = TileToSpawn->GetAttachLocation();
	}

}

void AInfiniteTerrainGameMode::PlayerConqueredTile()
{
	Score++;
}

void AInfiniteTerrainGameMode::GameOver(APawn * PlayerPawn)
{
	ACharacterPlayerController * PC = PlayerPawn ? Cast<ACharacterPlayerController>(PlayerPawn->GetController()) : nullptr;
	ALethalEntertainmentHUD * Hud = PC ? Cast<ALethalEntertainmentHUD>(PC->GetPlayerHud()) : nullptr;
	if (Hud)
	{
		Hud->DrawGameOverUI();
	}
}

