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

    AEnemy * TheChoosenEnemy ;
    UGameplayStatics::GetAllActorsOfClass(GetWorld() , AEnemy::StaticClass() , EnemiesArray);

    for(AActor * Actor : EnemiesArray)
    {
        TheChoosenEnemy = Cast<AEnemy>(Actor);
        
        //I have to destroy the weapons first and after that destroy the enemy so that the weapon don`t stay in the level when the enemies are destroyed
        if(TheChoosenEnemy->LeftMeleeWeapon)
        {
            TheChoosenEnemy->LeftMeleeWeapon->Destroy();
        }
        TheChoosenEnemy->RightMeleeWeapon->Destroy();
        TheChoosenEnemy->Destroy();
    }
    
}

