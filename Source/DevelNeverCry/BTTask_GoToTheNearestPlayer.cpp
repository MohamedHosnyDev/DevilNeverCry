// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GoToTheNearestPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_GoToTheNearestPlayer::UBTTask_GoToTheNearestPlayer()
{
    NodeName = TEXT("GoToTheNearestPlayer") ;
}

EBTNodeResult::Type UBTTask_GoToTheNearestPlayer::ExecuteTask(UBehaviorTreeComponent & BehaviorTree , uint8 * NodeMemory)
{
    Super :: ExecuteTask(BehaviorTree , NodeMemory) ;

    AIController = BehaviorTree.GetAIOwner() ;
    PlayerLocation = AIController -> GetBlackboardComponent() -> GetValueAsVector(TEXT("ThePlayerLocation")) ;

    Enemy = Cast<AEnemy>(AIController->GetPawn());

    if(Enemy->IsNormallyDamaged || Enemy->IsUpperDamaged || Enemy->IsStandingUp)
    {
        return EBTNodeResult::Failed ;
    }

    EnemyOldLocation = Enemy->GetActorLocation();
    OffsetVectorToPlayer = PlayerLocation - EnemyOldLocation ;
    DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) ;
    EnemyNewRotation = FRotator(Enemy->GetActorRotation().Pitch , OffsetVectorToPlayer.Rotation().Yaw - 90 , Enemy->GetActorRotation().Roll);
    EnemyNewLocation = EnemyOldLocation + (Enemy->GetActorRightVector() * Enemy->MoveSpeed * DeltaSeconds) ;

    Enemy->SetActorRotation(EnemyNewRotation);
    Enemy->SetActorLocation(EnemyNewLocation);
    Enemy->IsMoving = true ;
    Enemy->IsAttacking = false ;

    return EBTNodeResult::Succeeded;
}
