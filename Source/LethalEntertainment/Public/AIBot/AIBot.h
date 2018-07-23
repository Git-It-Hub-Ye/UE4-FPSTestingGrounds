// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Mannequin.h"
#include "AIBot.generated.h"


UCLASS()
class LETHALENTERTAINMENT_API AAIBot : public AMannequin
{
	GENERATED_BODY()
	
public:
	/** Aim ai character */
	void Aim();

	/** Stop ai character aim */
	void StopAim();
	
};
