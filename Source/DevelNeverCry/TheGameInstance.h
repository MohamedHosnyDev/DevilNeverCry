// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TheGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEVELNEVERCRY_API UTheGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	public:
		UPROPERTY(EditAnywhere)
		float PlayerHealth = 200 ;
		//This PlayerHealthLevel variable will be used in the upgrading widget so that the player can upgrade to each level one time only
		int PlayerHealthLevel ;
		//Extra red orbs amount = (PlayerCurrentCollectedRedOrbs - TargetRedOrbs) (in case that the player win the match) 
		int ExtraRedOrbsAmount ;

	public:
		UTheGameInstance();
		UFUNCTION(BlueprintPure)
		//This function is used in the UI
		int GetPlayerHealthLevel();
		UFUNCTION(BlueprintPure)
		//This function is used in the UI
		int GetExtraRedOrbsAmount() const ;
		UFUNCTION(BlueprintCallable)
		//This function is called when the player upgrade his health level
		void ReduceExtraRedOrbsAmount(int Amount) ;
		UFUNCTION(BlueprintCallable)
		//At Each health level upgrade the player health = LastMaxHealthValue * 2 and his health level value increase by 1
		void UpgradePlayerHealthToTheNextLevel();


};
