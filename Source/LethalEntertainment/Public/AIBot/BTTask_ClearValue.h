// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearValue.generated.h"


UCLASS()
class LETHALENTERTAINMENT_API UBTTask_ClearValue : public UBTTaskNode
{
	GENERATED_BODY()
	

protected:
	/** Value to be cleared */
	UPROPERTY(EditAnywhere, Category = "Condition")
	struct FBlackboardKeySelector ClearValueKey;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	

};
