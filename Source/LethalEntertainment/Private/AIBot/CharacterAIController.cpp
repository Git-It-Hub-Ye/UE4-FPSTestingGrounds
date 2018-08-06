// Fill out your copyright notice in the Description page of Project Settings.

#include "LethalEntertainment.h"
#include "AIBot.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "CharacterAIController.h"

ACharacterAIController::ACharacterAIController(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSense"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingSense"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageSense"));

	TeamId = FGenericTeamId(0);
}

void ACharacterAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		auto PossessedCharacter = Cast<AAIBot>(InPawn);

		if (!ensure(PossessedCharacter)) { return; }
		PossessedCharacter->OnCharacterDeath.AddUniqueDynamic(this, &ACharacterAIController::OnPossessedCharacterDeath);

		if (BlackboardToUse)
		{
			UseBlackboard(BlackboardToUse, BB);
		}
		if (BehaviorTreeToUse)
		{
			RunBehaviorTree(BehaviorTreeToUse);
		}

		if (PerceptionComp)
		{
			if (SightConfig)
			{
				SightConfig->DetectionByAffiliation.bDetectEnemies = true;
				SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
				SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
				PerceptionComp->ConfigureSense(*SightConfig);
				PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
			}
			if (HearingConfig)
			{
				HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
				HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
				HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;
				PerceptionComp->ConfigureSense(*HearingConfig);
			}
			if (DamageConfig)
			{
				PerceptionComp->ConfigureSense(*DamageConfig);
			}
			PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ACharacterAIController::SenseStuff);
		}

		EnemyKeyId = "Enemy";
		LastKnownLocationKeyId = "LastKnownLocation";
		CanShootKeyId = "CanShootEnemy";
		BB->SetValueAsBool(CanShootKeyId, true);
	}
}

bool ACharacterAIController::IsOnSameTile(AActor * Actor)
{
	AAIBot * AIBot = Cast<AAIBot>(GetPawn());
	AMannequin * Enemy = Cast<AMannequin>(Actor);

	if (AIBot && Enemy)
	{
		if (!AIBot->GetCurrentTile() || !Enemy->GetCurrentTile())
		{
			AIBot->UpdateTile();
			Enemy->UpdateTile();
		}
		if (AIBot->GetCurrentTile() && Enemy->GetCurrentTile() && AIBot->GetCurrentTile() == Enemy->GetCurrentTile())
		{
			return true;
		}
	}

	return false;
}

ETeamAttitude::Type ACharacterAIController::GetTeamAttitudeTowards(const AActor & Other) const
{
	if (const APawn * OtherCharacter = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface * TeamAgent = Cast<IGenericTeamAgentInterface>(OtherCharacter->GetController()))
		{
			return Super::GetTeamAttitudeTowards(*OtherCharacter->GetController());;
		}
	}
	return ETeamAttitude::Neutral;
}

void ACharacterAIController::SenseStuff(AActor * SensedActor, FAIStimulus Stimulus)
{
	if (SensedActor == GetPawn()) { return; }
	if (Stimulus.Type == UAISense::GetSenseID<UAISenseConfig_Hearing>() && !IsOnSameTile(SensedActor)) { return; }
	
	ETeamAttitude::Type AttitudeTowards = GetTeamAttitudeTowards(*SensedActor);
	
	if (Stimulus.WasSuccessfullySensed())
	{
		if (BlackboardToUse && BB->GetValueAsObject(EnemyKeyId)) { return; }
		if (AttitudeTowards == ETeamAttitude::Hostile && BlackboardToUse)
		{
			if (Stimulus.Type == UAISense::GetSenseID<UAISenseConfig_Sight>())
			{
				BB->SetValueAsObject(EnemyKeyId, SensedActor);
				SetFocus(SensedActor);
			}
			else
			{
				BB->SetValueAsVector(LastKnownLocationKeyId, SensedActor->GetActorLocation());
			}
		}
	}
	else
	{
		if (AttitudeTowards == ETeamAttitude::Hostile && BlackboardToUse)
		{
			BB->ClearValue(EnemyKeyId);
		}
	}
}

void ACharacterAIController::ShootEnemy()
{
	AAIBot * AIBot = Cast<AAIBot>(GetPawn());

	bool bCanShoot = false;
	bool bHasLOS = true;
	AMannequin * Enemy = Cast<AMannequin>(BB->GetValueAsObject(EnemyKeyId));

	if (Enemy)
	{
		if (!AIBot->GetIsBlocked())
		{
			bCanShoot = true;
		}
		if (!LineOfSightTo(Enemy, GetPawn()->GetActorLocation()))
		{
			bHasLOS = false;
		}
	}

	if (bCanShoot)
	{
		AIBot->PullTrigger();
		BB->SetValueAsBool(CanShootKeyId, true);
	}
	else
	{
		AIBot->ReleaseTrigger();
	}

	if (!bHasLOS)
	{
		BB->SetValueAsBool(CanShootKeyId, false);
	}
}

void ACharacterAIController::StopFire()
{
	AAIBot * AIBot = Cast<AAIBot>(GetPawn());

	AIBot->ReleaseTrigger();
}

void ACharacterAIController::AimWeapon(bool bAimWeapon)
{
	AAIBot * AIBot = Cast<AAIBot>(GetPawn());

	bAimWeapon ? AIBot->Aim() : AIBot->StopAim();
}

void ACharacterAIController::UpdateLastKnownLocation()
{
	APawn * Enemy = Cast<APawn>(BB->GetValueAsObject(EnemyKeyId));

	if (Enemy)
	{
		BB->SetValueAsVector(LastKnownLocationKeyId, Enemy->GetActorLocation());
	}
	else
	{
		BB->ClearValue(LastKnownLocationKeyId);
	}
}

void ACharacterAIController::OnPossessedCharacterDeath()
{
	if (!GetPawn()) { return; }
	GetPawn()->DetachFromControllerPendingDestroy();
}

