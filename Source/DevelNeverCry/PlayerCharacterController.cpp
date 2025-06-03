// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"



void APlayerCharacterController::BeginPlay()
{
    Super::BeginPlay() ;

    PlayGameplayWidget() ;
}

void APlayerCharacterController::PlayGameplayWidget()
{
    if(TheGameplayWidget)
    {
        GameplayWidget = CreateWidget(GetWorld(),TheGameplayWidget);
        GameplayWidget -> AddToViewport();
    }
}

void APlayerCharacterController::RemoveGameplayWidget()
{
    if(TheGameplayWidget)
    {
        GameplayWidget -> RemoveFromParent();
    }
}

void APlayerCharacterController::GameHasEnded(AActor * FocusActor , bool PlayerIsWinner)
{
    Super::GameHasEnded(FocusActor , PlayerIsWinner);

    AEnemy * TheEnemy ;
    UGameplayStatics::GetAllActorsOfClass(GetWorld() , AEnemy::StaticClass() , EnemiesArray);

    for(AActor * Actor : EnemiesArray)
    {
        TheEnemy = Cast<AEnemy>(Actor);
        
        //I have to destroy the weapons first and after that destroy the enemy so that the weapon don`t stay in the level when the enemies are destroyed
        if(TheEnemy->LeftMeleeWeapon)
        {
            TheEnemy->LeftMeleeWeapon->Destroy();
        }
        TheEnemy->RightMeleeWeapon->Destroy();
        TheEnemy->Destroy();
    }
    
}

