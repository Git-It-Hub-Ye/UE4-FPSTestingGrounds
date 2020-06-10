// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "ActorPool.h"


AActor * UActorPool::Checkout()
{
	if (Pool.Num() == 0) {
		return nullptr;
	}
	return Pool.Pop();
}

void UActorPool::Return(AActor * ActorToReturn)
{
	Add(ActorToReturn);
}

void UActorPool::Add(AActor * ActorToAdd)
{
	Pool.Push(ActorToAdd);
}

