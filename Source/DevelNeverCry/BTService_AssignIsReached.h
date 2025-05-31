// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "AIController.h"
#include "GameFramework/PlayerController.h"
#include "BTService_AssignIsReached.generated.h"



UCLASS()
class DEVELNEVERCRY_API UBTService_AssignIsReached : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
	private:
		AAIController * AIController ;
		APawn * Enemy ;
		FVector EnemyLocation ;
		
		APlayerController * PlayerController ;
		AActor * ThePlayer ;
		FVector PlayerLocation ;

		UBTService_AssignIsReached();
		virtual void TickNode (UBehaviorTreeComponent & BehaviorTree , uint8 * NodeMemory , float DeltaTime) override ;


};
