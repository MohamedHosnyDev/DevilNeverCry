// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AIController.h"
#include "Enemy.h"
#include "BTTask_GoToTheNearestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class DEVELNEVERCRY_API UBTTask_GoToTheNearestPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	private:
		AAIController * AIController ;
		FVector PlayerLocation ;
		AEnemy * Enemy ;

		float DeltaSeconds ;
		FVector EnemyOldLocation ;
		FVector EnemyNewLocation ;
		FRotator EnemyNewRotation ;
		FVector OffsetVectorToPlayer ;

		UBTTask_GoToTheNearestPlayer();
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & BehaviorTree , uint8 * NodeMemory)override; 


	
};
