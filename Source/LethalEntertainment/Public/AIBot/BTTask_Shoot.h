// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Shoot.generated.h"


UCLASS()
class LETHALENTERTAINMENT_API UBTTask_Shoot : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	/** Set Ai to shoot weapon (true = shoot) */
	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bSetShoot;
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};
