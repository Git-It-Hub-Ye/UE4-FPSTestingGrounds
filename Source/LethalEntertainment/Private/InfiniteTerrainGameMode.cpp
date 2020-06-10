// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"
#include "Terrain/Tile.h"
#include "Player/CharacterPlayerController.h"
#include "LethalGameInstance.h"
#include "LethalEntertainmentHUD.h"
#include "Game/LethalSaveGame.h"
#include "InfiniteTerrainGameMode.h"


AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
	PrimaryActorTick.bCanEverTick = false;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Dynamic/Character/Behaviour/BP_PlayerPawn"));
	if (PlayerPawnClassFinder.Class)
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<ULethalSaveGame> DefaultSaveGameClass(TEXT("/Game/Dynamic/Blueprints/BP_SaveGame"));
	if (DefaultSaveGameClass.Class)
	{
		SaveGameClass = DefaultSaveGameClass.Class;
	}
	
	// use our custom HUD class
	HUDClass = ALethalEntertainmentHUD::StaticClass();

	NavMeshPool = CreateDefaultSubobject<UActorPool>(FName("NavMeshPool"));

	InitialTilesToSpawn = 1;
	SpawnTileTransform.SetLocation(FVector(0.f, 0.f, 0.f));
	Score = 0;
	HighScore = 0;
}

void AInfiniteTerrainGameMode::BeginPlay()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlot1, 0))
	{
		LoadGameData();
	}
	else
	{
		SaveGameData();
	}

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

void AInfiniteTerrainGameMode::GameOver(APlayerController * PC)
{
	if (Score > HighScore) 
	{ 
		HighScore = Score; 
		SaveGameData(); 
	}

	if (!PC) { return; }
	ULethalGameInstance * GI = Cast<ULethalGameInstance>(GetGameInstance());

	if (GI)
	{
		GI->DisplayGameOverMenu();
	}
}

void AInfiniteTerrainGameMode::LoadGameData()
{
	SaveGameRef = Cast<ULethalSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot1, 0));
	if (SaveGameRef)
	{
		HighScore = SaveGameRef->Saved_Highscore;
	}
}

void AInfiniteTerrainGameMode::SaveGameData()
{
	if (!SaveGameRef->IsValidLowLevel())
	{
		if (SaveGameClass)
		{
			SaveGameRef = Cast<ULethalSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
			if (SaveGameRef)
			{
				UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveSlot1, 0);
			}
		}
	}
	else if (SaveGameRef->IsValidLowLevel())
	{
		SaveGameRef->Saved_Highscore = HighScore;
		UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveSlot1, 0);
	}
}

