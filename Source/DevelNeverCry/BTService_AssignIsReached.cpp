// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AssignIsReached.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerCharacter.h"


UBTService_AssignIsReached::UBTService_AssignIsReached()
{
    NodeName = TEXT("AssignIsReachedAndPlayerLocation") ;
}

void UBTService_AssignIsReached::TickNode (UBehaviorTreeComponent & BehaviorTree , uint8 * NodeMemory , float DeltaTime)
{
    Super :: TickNode (BehaviorTree , NodeMemory , DeltaTime);

    AIController = BehaviorTree.GetAIOwner() ;
    Enemy = AIController->GetPawn() ;
    EnemyLocation = Enemy->GetActorLocation();

    PlayerController = GetWorld() -> GetFirstPlayerController() ;
    ThePlayer = PlayerController -> GetPawn() ;

    PlayerLocation = ThePlayer->GetActorLocation();

    AIController -> GetBlackboardComponent() -> SetValueAsVector(TEXT("ThePlayerLocation"),PlayerLocation);

    if(FVector::Dist(EnemyLocation , PlayerLocation) <= 150)
    {
        AIController -> GetBlackboardComponent() -> SetValueAsBool(TEXT("IsReached"),true);
    }
    else
    {
        AIController -> GetBlackboardComponent() -> SetValueAsBool(TEXT("IsReached"),false);
    }


}