// Copyright 2018 - 2021 Stuart McDonald.

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
