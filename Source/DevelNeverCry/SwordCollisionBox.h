// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "SwordCollisionBox.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom) , meta = (BlueprintSpawnableComponent))
class DEVELNEVERCRY_API USwordCollisionBox : public UBoxComponent
{
	GENERATED_BODY()
	
	public :
		//This is an array of all the overlapping actors in the box
		TArray<AActor*> Actors ;

		//Get the first overlapping Character or Enemy
		AActor * GetOverlappingCharacter() ;	

};
