// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGameInstance.h"


UTheGameInstance::UTheGameInstance()
{
    //The default player health level is level 1
    PlayerHealthLevel = 1 ;
    ExtraRedOrbsAmount = 0 ;
}

int UTheGameInstance::GetPlayerHealthLevel()
{
    return PlayerHealthLevel ;
}

int UTheGameInstance::GetExtraRedOrbsAmount() const 
{
    return ExtraRedOrbsAmount ;
}

void UTheGameInstance::ReduceExtraRedOrbsAmount(int Amount) 
{
    ExtraRedOrbsAmount -= Amount ;
}

void UTheGameInstance::UpgradePlayerHealthToTheNextLevel()
{
    PlayerHealthLevel ++ ;
    PlayerHealth *= 2 ;
}
