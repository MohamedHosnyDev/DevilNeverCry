// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class DEVELNEVERCRY_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	public:
		AEnemy * Enemy ;
	
	protected:
		virtual void BeginPlay() override ;
	
	public :
		AEnemyController() ;
		
		virtual void Tick(float DeltaTime) override ;
};
