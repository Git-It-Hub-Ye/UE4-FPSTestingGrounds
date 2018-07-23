// Fill out your copyright notice in the Description page of Project Settings.

#include "LethalEntertainment.h"
#include "CharacterAIController.h"
#include "BTTask_AimWeapon.h"

EBTNodeResult::Type UBTTask_AimWeapon::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ACharacterAIController * AICon = Cast<ACharacterAIController>(OwnerComp.GetAIOwner());
	if (AICon)
	{
		AICon->AimWeapon(bSetAim);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
