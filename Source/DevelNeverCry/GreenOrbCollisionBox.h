// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "GreenOrbCollisionBox.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom) , meta = (BlueprintSpawnableComponent))
class DEVELNEVERCRY_API UGreenOrbCollisionBox : public UBoxComponent
{
	GENERATED_BODY()

	public:
		//This is an array of all the overlapping actors in the box
		TArray<AActor*> OverlappingActors;
		APlayerCharacter * Player ;
		UPROPERTY(EditDefaultsOnly)
		UParticleSystem * CollectParticles ;
		UPROPERTY(EditDefaultsOnly)
		USoundBase * CollectSound ;

		UGreenOrbCollisionBox();
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override ;

};
