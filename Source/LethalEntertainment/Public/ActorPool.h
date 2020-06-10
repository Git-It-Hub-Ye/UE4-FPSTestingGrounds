// Copyright 2018 - 2020 Stuart McDonald.

#pragma once

#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETHALENTERTAINMENT_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

private:
	/** Stores each nav mesh instance */
	TArray<AActor*> Pool;

public:
	/** Returns a nav mesh from array */
	AActor * Checkout();

	/** Gets the actor to be returned */
	void Return(AActor * ActorToReturn);

	/** Returns nav mesh back to array */
	void Add(AActor * ActorToAdd);
	
};
