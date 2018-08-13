// Copyright 2018 Stuart McDonald.

#include "LethalEntertainment.h"
#include "InfiniteTerrainGameMode.h"
#include "LethalEntertainmentHUD.h"
#include "Mannequin.h"
#include "CharacterPlayerController.h"


ACharacterPlayerController::ACharacterPlayerController(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamId = FGenericTeamId(1);
}

void ACharacterPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedCharacter = Cast<AMannequin>(InPawn);

		if (!ensure(PossessedCharacter)) { return; }
		PossessedCharacter->OnCharacterDeath.AddUniqueDynamic(this, &ACharacterPlayerController::OnPossessedCharacterDeath);
	}
}

void ACharacterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseGame", IE_Released, this, &ACharacterPlayerController::PauseGameMenu).bExecuteWhenPaused = true;
}

void ACharacterPlayerController::FindDeathCameraSpot(FVector & StartLocation)
{
	FVector EndLocation = StartLocation + FVector(0.f, 0.f, 300.f);
	FRotator ViewDir = GetControlRotation();
	ViewDir.Pitch = -90.f;

	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Camera
	);
	
	FVector EndPoint = HitResult.GetActor() ? HitResult.ImpactPoint + 10.f : EndLocation;
	BlendToDeathCamera(EndPoint, ViewDir);
}

void ACharacterPlayerController::BlendToDeathCamera(FVector CamLoc, FRotator CamRot)
{
	SetInitialLocationAndRotation(CamLoc, CamRot);
	SetViewTarget(this);
}

float ACharacterPlayerController::CalculateTargetRotaion(float CurrentTarget, float CurrentPitch)
{
	GetInputMouseDelta(DeltaX, DeltaY);

	if (DeltaY > 0.f)
	{
		NewTargetRot += DeltaY;

		// Stop over rotation
		NewTargetRot = NewTargetRot > 89.f ? 89.f : NewTargetRot;
	}
	else if (DeltaY < 0.f)
	{
		NewTargetRot = CurrentPitch < CurrentTarget ? CurrentPitch : CurrentTarget;
	}
	else
	{
		NewTargetRot = CurrentTarget;
	}

	return NewTargetRot;
}

ALethalEntertainmentHUD * ACharacterPlayerController::GetPlayerHud() const
{
	return Cast<ALethalEntertainmentHUD>(GetHUD());
}

void ACharacterPlayerController::PauseGameMenu()
{
	AMannequin * MyPawn = Cast<AMannequin>(GetPawn());
	ALethalEntertainmentHUD * Hud = GetPlayerHud();

	if (MyPawn && !MyPawn->IsDead() && Hud)
	{
		if (!IsPaused())
		{
			MyPawn->ResetInputOnPause();
		}
		Hud->DrawPauseUI();
	}
}

void ACharacterPlayerController::OnPossessedCharacterDeath()
{
	AInfiniteTerrainGameMode * GM = Cast<AInfiniteTerrainGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		GM->GameOver(GetPawn());
	}

	FVector DeathLocation = GetPawn()->GetActorLocation();
	UnPossess();
	FindDeathCameraSpot(DeathLocation);
}
