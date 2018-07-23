// Fill out your copyright notice in the Description page of Project Settings.

#include "LethalEntertainment.h"
#include "AIBot.h"

void AAIBot::Aim()
{
	bIsAiming = true;
}

void AAIBot::StopAim()
{
	bIsAiming = false;
}
