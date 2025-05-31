// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AIController.h"
#include "Enemy.h"
#include "BTTask_AttackThePlayer.generated.h"



UCLASS()
class DEVELNEVERCRY_API UBTTask_AttackThePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	private :
		AAIController * AIController ;
		AEnemy * Enemy ;
		FVector EnemyLocation ;
		FRotator EnemyRotator ;
		FVector OffsetVectorToPlayer ;
		FVector PlayerLocation ;

		UBTTask_AttackThePlayer();
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & BehaviorTree , uint8 * NodeMemory)override ;

	
};
