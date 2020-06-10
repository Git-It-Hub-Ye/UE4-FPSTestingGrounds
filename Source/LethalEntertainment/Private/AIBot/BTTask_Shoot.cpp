// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "CharacterAIController.h"
#include "BTTask_Shoot.h"

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ACharacterAIController * AICon = Cast<ACharacterAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		bSetShoot ? AICon->ShootEnemy() : AICon->StopFire();
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
