// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AimWeapon.generated.h"


UCLASS()
class LETHALENTERTAINMENT_API UBTTask_AimWeapon : public UBTTaskNode
{
	GENERATED_BODY()


protected:
	/** Set Ai to aim weapon (true = Aim) */
	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bSetAim;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);



	
};
