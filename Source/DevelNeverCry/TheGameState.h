// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"
#include "TheGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "TheGameState.generated.h"

/**
 * 
 */
UCLASS()
class DEVELNEVERCRY_API ATheGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	public:
		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget>TheWinWidget ;
		UUserWidget * WinWidget ;
		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget>TheLoseWidget ;
		UUserWidget * LoseWidget ;

		UPROPERTY(EditAnywhere)
		//The player must collect this number of red orbs to win the level
		int TargetRedOrbsNumber = 300 ;
		//This time handler is used to decrease the Remaining time every 1 second
		FTimerHandle DecreaseTimeHandler ;
		UPROPERTY(EditAnywhere)
		//This is the max time in seconds that the player must collect the target number of red orbs before it ends
		int TargetMaxTime = 300 ;
		/* I set RemainingTime to 1 as a temporary value while debugging an issue.
		The bug was caused by RemainingTime not being initialized in the header file correctly
		To fix this, I assigned it to TargetMaxTime in the BeginPlay function, which resolved the problem */
		int RemainingTime = 1 ;
		APlayerCharacterController * PlayerController ;
		APlayerCharacter * Player ;
		UTheGameInstance * GameInstance ;
		//GameEnded bool variable is used to avoid calling GameHasEnded() every frame in the tick function if the game already ended
		bool GameEnded ;

	protected:
		virtual void BeginPlay() override ;

	public:
		ATheGameState();
		virtual void Tick(float DeltaTime) override ;

		//The following 3 functions will be used in the gameplay widget declared in the player character controller
		UFUNCTION(BlueprintPure)
		int GetTargetRedOrbsNumber() const ;
		UFUNCTION(BlueprintPure)
		//The number of minutes that will return is the result of :: (RemainingTime - (RemainingTime % 60)) / 60
		int GetRemainingTimeMinutes() const ;
		UFUNCTION(BlueprintPure)
		//The number of seconds that will return is the result of :: RemainingTime % 60
		int GetRemainingTimeSeconds() const ;
		//This function is used to check if the player wins or lose
		void CheckWinAndLoseConditions();
		//Show win widget
		void PlayWinWidget();
		//Show lose widget
		void PlayLoseWidget();
		void DecreaseTime();

	
};
