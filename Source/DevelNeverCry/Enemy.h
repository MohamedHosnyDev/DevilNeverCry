// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MeleeWeapon.h"
#include "Enemy.generated.h"



UCLASS()
class DEVELNEVERCRY_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	//This widget will show the enemy health bar above him
	UWidgetComponent * HealthWidget ;
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree * AIBehaviorTree ;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMeleeWeapon>TheMeleeWeapon ;
	AMeleeWeapon * RightMeleeWeapon ;
	AMeleeWeapon * LeftMeleeWeapon ;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> RedOrbActor ;

	FTimerHandle SpawnRedOrbTimer ;
	//Used when the enemy is reviving(and to set IsMoving to true)
	FTimerHandle ReviveTimer ;
	FTimerHandle DamageTimer ;

	UPROPERTY(EditDefaultsOnly)
	//Each kind of enemies will have special max health
	float MaxHealth = 100 ;
	/* I set CurrentHealth to 1 as a temporary value while debugging an issue.
	The bug was caused by CurrentHealth not being initialized in the header file correctly
	To fix this, I assigned it to MaxHealth in the BeginPlay function, which resolved the problem */
	float CurrentHealth = 1 ;
	UPROPERTY(EditInstanceOnly)
	float MoveSpeed = 450 ;
	//Enemy old Z axis value will be used to know if the enemy is in the air (Upper damaged) or no
	float EnemyOldZAxisPositionValue ;
	// IsReviving is used so that the compiler don`t set timer more than one time when tthe enemy is dead
	bool IsReviving ;
	// The next 6 bool variables is used to determine which animation will be applied 
	bool IsMoving ;
	bool IsAttacking ;
	//There is two kinds of attacks that the player can do with the sword ,normal attack and the upper attack which will make the enemy fly in the air 
	bool IsNormallyDamaged ;
	bool IsUpperDamaged ;
	bool IsDead ;
	//When the enemy stands up after being revived
	bool IsStandingUp ;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	AEnemy();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// This function is used to spawn red orbs and revive the enemy if he is dead by checking in a loop timer if he is dead 
	void SpawnRedOrbsAndReviveEnemy();
	//Used in he BeginPlay function to spawn the melee weapon in one or two hands of the enemy
	void SpawnWeapon() ;

	//When the enemy dies , after one minute he will make his max health full again and rise again
	void Revive() ;
	//in case of shooting weapons the SwordAttackIndex = 0
	void ActivateDamageAnimation(int SwordAttackIndex);
	//This two functions will be called by the player when he Aim or DisableAim on an enemy to show or hide enemy health widget
	void ShowHealthWidget();
	void HideHealthWidget();
	//Used in the enemy health bar
	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const ;
	//I know that if i made the bool variable uproperty BlueprintReadOnly , it will automatically make Get Functions for every one of them put it is more professional to make get function so i made them 
	UFUNCTION(BlueprintPure)
	bool GetIsMoving() const ;
	//Set IsMoving to true
	void SetIsMovingTrue();
	UFUNCTION(BlueprintPure)
	bool GetIsAttacking() const ;
	UFUNCTION(BlueprintPure)
	bool GetIsNormallyDamaged() const ;
	UFUNCTION(BlueprintPure)
	bool GetIsUpperDamaged() const ;
	//This function is used to set both of the NormallyDameged and the UpperDamaged
	void SetIsDamagedToFalse();
	UFUNCTION(BlueprintPure)
	bool GetIsDead();

};
