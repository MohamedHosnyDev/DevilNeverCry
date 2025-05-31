// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemyController::AEnemyController()
{
    PrimaryActorTick.bCanEverTick = true ;
}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();
    
    Enemy = Cast<AEnemy>(GetPawn());
    
    if(Enemy->AIBehaviorTree)
    {
        RunBehaviorTree(Enemy->AIBehaviorTree);
    }
}

void AEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Enemy->AIBehaviorTree)
    {
        GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"),Enemy->IsDead);
    }
}