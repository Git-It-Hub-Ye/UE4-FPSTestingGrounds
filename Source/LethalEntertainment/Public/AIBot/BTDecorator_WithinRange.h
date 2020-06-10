// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_WithinRange.generated.h"


UCLASS()
class LETHALENTERTAINMENT_API UBTDecorator_WithinRange : public UBTDecorator
{
	GENERATED_BODY()

protected:
	/** Value of current enemy */
	UPROPERTY(EditAnywhere, Category = "Condition")
	struct FBlackboardKeySelector EnemyKey;

	/** Target radius for ai to enemy */
	UPROPERTY(EditAnywhere, Category = "Condition")
	float WithinRange;

public:
	UBTDecorator_WithinRange();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** Checks if ai is within a certain radius to enemy */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};
