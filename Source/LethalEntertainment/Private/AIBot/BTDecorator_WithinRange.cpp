// Fill out your copyright notice in the Description page of Project Settings.

#include "LethalEntertainment.h"
#include "CharacterAIController.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "BTDecorator_WithinRange.h"

UBTDecorator_WithinRange::UBTDecorator_WithinRange()
{
	bNotifyTick = true;
}

void UBTDecorator_WithinRange::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (!CalculateRawConditionValue(OwnerComp, NodeMemory))
	{
		OwnerComp.RequestExecution(this);
	}
}

bool UBTDecorator_WithinRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	ACharacterAIController * AICon = Cast<ACharacterAIController>(OwnerComp.GetAIOwner());

	if (AICon && BB)
	{
		auto KeyValue = BB->GetValueAsObject(EnemyKey.SelectedKeyName);
		AActor * Enemy = Cast<AActor>(KeyValue);

		if (Enemy)
		{
			FVector GetDistance = AICon->GetPawn()->GetActorLocation() - Enemy->GetActorLocation();
			float DistanceToEnemy = GetDistance.Size();

			return DistanceToEnemy <= WithinRange;
		}
		return false;
	}
	return false;
}

