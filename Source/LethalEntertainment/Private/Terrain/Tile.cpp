// Fill out your copyright notice in the Description page of Project Settings.

#include "LethalEntertainment.h"
#include "ActorPool.h"
#include "Components/ArrowComponent.h"
#include "Tile.h"

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	SpawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnLocation"));
	SpawnLocation->SetupAttachment(RootComponent);
	SpawnLocation->SetRelativeLocation(FVector(4000.f, -3000.f, 0.f));

	NavMeshOffset = FVector(2000, -2000, 0);
	MinExtent = FVector(300, -3700, 0);
	MaxExtent = FVector(3700, 0, 0);
}

void ATile::SetPool(class UActorPool * InPool)
{
	Pool = InPool;
	PositionNavMesh();
}

void ATile::PositionNavMesh()
{
	NavMesh = Pool->Checkout();

	if (NavMesh == nullptr) {
		return;
	}
	NavMesh->SetActorLocation(GetActorLocation() + NavMeshOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

FTransform ATile::GetAttachLocation() const
{
	return SpawnLocation->GetComponentTransform();
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, FSpawnParameters SpawnParameters)
{
	RandomlyPlaceActors(ToSpawn, SpawnParameters);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, FSpawnParameters SpawnParameters)
{
	RandomlyPlaceActors(ToSpawn, SpawnParameters);
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, FSpawnParameters SpawnParameters)
{
	int NumberToSpawn = FMath::RandRange(SpawnParameters.MinSpawn, SpawnParameters.MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++) {
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(SpawnParameters.MinScale, SpawnParameters.MaxScale);
		bool found = FindEmptyLocation(SpawnPosition.Location, SpawnParameters.Radius * SpawnPosition.Scale);
		if (found) {
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

bool ATile::FindEmptyLocation(FVector & OutLocation, int32 Radius)
{
	FBox Bounds(MinExtent, MaxExtent);

	const int32 MAX_ATTEMPTS = 10;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius)) {
			OutLocation = CandidatePoint;
			return true;
		}
	}
	return false;
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		COLLISION_SPAWNED,
		FCollisionShape::MakeSphere(Radius)
	);
	return !HasHit;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition & SpawnPosition)
{
	AActor * Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);

	if (Spawned) {
		GarbageProps.Add(Spawned);
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	}

	return;
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition & SpawnPosition)
{
	FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);
	APawn * Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location, Rotation);

	if (Spawned == nullptr) { return; }

	GarbageProps.Add(Spawned);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SpawnDefaultController();
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Pool != nullptr && NavMesh != nullptr) {
		Pool->Return(NavMesh);
	}

	if (GarbageProps.Num() != 0)
	{
		AActor * Prop = nullptr;

		while (GarbageProps.Num() != 0)
		{
			Prop = GarbageProps.Pop();

			if (Prop != NULL)
			{
				Prop->Destroy();
			}
		}
	}
}

