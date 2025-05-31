// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackThePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTTask_AttackThePlayer::UBTTask_AttackThePlayer()
{
    NodeName = TEXT("AttackThePlayer");
}

EBTNodeResult::Type UBTTask_AttackThePlayer::ExecuteTask(UBehaviorTreeComponent & BehaviorTree , uint8 * NodeMemory)
{
    Super :: ExecuteTask(BehaviorTree , NodeMemory);

    AIController = BehaviorTree.GetAIOwner() ;
    Enemy = Cast<AEnemy>(AIController->GetPawn());

    if(Enemy->IsNormallyDamaged || Enemy->IsUpperDamaged || Enemy->IsStandingUp)
    {
        return EBTNodeResult::Failed;
    }

    EnemyLocation = Enemy->GetActorLocation();
    PlayerLocation = AIController->GetBlackboardComponent()->GetValueAsVector(TEXT("ThePlayerLocation"));
    OffsetVectorToPlayer = PlayerLocation - EnemyLocation ;
    EnemyRotator = FRotator(Enemy->GetActorRotation().Pitch , OffsetVectorToPlayer.Rotation().Yaw - 90 , Enemy->GetActorRotation().Roll);

    Enemy->SetActorRotation(EnemyRotator);
    Enemy->IsMoving = false ;
    Enemy->IsAttacking = true ;

    return EBTNodeResult::Succeeded ;

}