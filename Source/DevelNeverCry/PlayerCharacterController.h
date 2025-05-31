// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacterController.generated.h"



UCLASS()
class DEVELNEVERCRY_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
	public:
		TArray<AActor*> EnemiesArray ;
		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> TheGameplayWidget ;
		UUserWidget * GameplayWidget ;

		//The next two functions is used to show and remove the Gameplay Widget
		void PlayGameplayWidget();
		void RemoveGameplayWidget();
		//This Function is called to end the game and disconnect The player and the enemies from there controllers and show win or lose widget after removing the Gameplay widget 
		virtual void GameHasEnded(AActor * FocusActor , bool PlayerIsWinner) override ;
	
	protected:

		virtual void BeginPlay() override ;

};
