// Fill out your copyright notice in the Description page of Project Settings.

#include "LethalEntertainment.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "BTTask_ClearValue.h"

EBTNodeResult::Type UBTTask_ClearValue::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UBlackboardComponent * BB = OwnerComp.GetBlackboardComponent();
	if (BB)
	{
		BB->ClearValue(ClearValueKey.SelectedKeyName);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
