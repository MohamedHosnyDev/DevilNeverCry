// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TheGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "MeleeWeapon.h"
#include "ShootingWeapon.h"
#include "Enemy.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DEVELNEVERCRY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/* We will use the GameInstance to get the player max health -
	because the player will be able to upgrade his max health level in the main menu */

	UTheGameInstance * GameInstance ;
	//We will use the Skeletal Mesh when the player aim on the enemies or move in the level
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent * SkeletalMesh ;
	UPROPERTY(EditAnywhere)
	USpringArmComponent * SpringArm ;
	UPROPERTY(EditAnywhere)
	UCameraComponent * Camera ;

	//The FightingStyles enum will be used to activate this styles changes on weapons
	enum FightingStyle {SwordMaster , GunSlinger} ;
	//The default Style is SwordMaster because the sword is the default spawned weapon
	FightingStyle Style = SwordMaster ;
	UPROPERTY(EditDefaultsOnly)
	USoundBase * SwitchToSwordMasterStyleSound ;
	UPROPERTY(EditDefaultsOnly)
	USoundBase * SwitchToGunSlingerStyleSound ;
	UPROPERTY(EditDefaultsOnly)
	USoundBase * SwitchWeaponSound ;
	UPROPERTY(EditDefaultsOnly)
	USoundBase * DamageSound ;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMeleeWeapon> TheSword ;
	AMeleeWeapon * Sword ;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AShootingWeapon> TheGun ;
	AShootingWeapon * RightGun ;
	AShootingWeapon * LeftGun ;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AShootingWeapon> TheShotGun ;
	AShootingWeapon * ShotGun ;
	//HasChoosedAimingEnemy boolen variable is used to choose the enemy for only one time
	bool HasChoosedAimingEnemy ;
	//This timer handler will be used to delay the shot gun shot a bit
	FTimerHandle ShotGunTimerHandler ;
	//This bool variable is used to delay between the ShotGun shots
	bool CanUseShotGun ;
	//This FString variable will help in choosing which animation to play ,It`s default value is the TEXT("Sword") because the sword is the default spawned weapon
	FString SpawnedWeaponName ;
	//The next two actor variables will be used to destroy the current spawned weapons when the player switch between weapons
	AActor * RightSpawnedWeapon ;
	AActor * LeftSpawnedWeapon ;
	//Used to determine which sword attack animation is used for the player or the damaged enemy(knowing that the max value = 4 , and it is used in the upper attack)
	int SwordAttackIndex ;
	//In case of using guns there will be left and right gun , this index is the index of the gun that the player will use this time (It changes after every gun shot and the max value = 2)
	int GunIndex ;

	/* I have to give the MaxHealth and the CurrentHealth any default value to avoid engine crash (1 as a temporary value)
	in the BeginPlay we assign the GameInstance PlayerHealth to the MaxHealth and make CurrentHealth = MaxHealth */
	float MaxHealth = 1 ;
	float CurrentHealth = 1 ;
	int CurrentCollectedRedOrbsAmount ;

	//This actor will be used when the player call the aim function to point to the nearest enemy to the player
	AEnemy * NearestEnemy ;
	//This transform will be used to reset the camera after aiming
	FTransform SpringArmOldTransform;

	//This bool variables is used to activate animations
	struct FAnimationBoolVariables
	{
		bool IsDamaged ;
		bool IsAttackingWithSword ;
		bool IsAimingWithGun ;
		bool IsAimingWithShotGun ;
		bool IsSwitchingToGunSlingerStyle ;
		bool IsSwitchingToSwordMasterStyle ;
		bool IsDead ;
	};
	FAnimationBoolVariables BoolVariables ;
	//This timer handlers are used to set the Animation boolean variables to false when needed
	FTimerHandle AnimationTimerHandler ;
	FTimerHandle DamageTimerHandler ;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	int GetSwordAttackIndex() const ;
	int GetGunIndex() const ;
	void ActivateDamageAnimationAndSound();
	//This function will be used by the GreenOrbCollisionBox to increase the player health
	void IncreasePlayerHealth();
	//This function will be used by the RedOrbCollisionBox to increase the collected red orbs amount
	void IncreaseCollectedRedOrbsAmount();
	UFUNCTION(BlueprintPure)
	//Used in the UI
	float GetPlayerHealthPercentage() const ;
	UFUNCTION(BlueprintPure)
	//Used in the UI
	int GetCurrentCollectedRedOrbsAmount() const ;

	UFUNCTION(BlueprintPure)
	/* Extra red orbs amount = (PlayerCurrentCollectedRedOrbs - TargetRedOrbs) (in case that the player win the match or get extra orbs) - 
	This function will be called in the game state and assign the return value to the game instance ExtraRedOrbsAmount Variable */
	int GetExtraRedOrbs() const ;
	UFUNCTION(BlueprintPure)
	//Used in the enemy aiming widget
	float GetAimedAtEnemyHealthPercentage() const ;
	UFUNCTION(BlueprintPure)
	bool GetIsDamaged() const ;
	void SetIsDamagedToFalse();
	UFUNCTION(BlueprintPure)
	bool GetIsAttackingWithSword() const ;
	void SetIsAttackingWithSwordToFalse();
	UFUNCTION(BlueprintPure)
	bool GetIsAimingWithGun() const ;
	UFUNCTION(BlueprintPure)
	bool GetIsAimingWithShotGun() const ;
	UFUNCTION(BlueprintPure)
	//Used in the UI
	FString GetSpawnedWeaponName() const ;
	UFUNCTION(BlueprintPure)
	//Used in the UI
	FString GetFightingStyleName() const ;
	UFUNCTION(BlueprintPure)
	bool GetIsSwitchingToGunSlingerStyle() const ;
	void SetIsSwitchingToGunSlingerStyleToFalse(); 
	UFUNCTION(BlueprintPure)
	bool GetIsSwitchingToSwordMasterStyle() const ;
	void SetIsSwitchingToSwordMasterStyleToFalse(); 
	UFUNCTION(BlueprintPure)
	bool GetIsDead();

private:
	//This function is used to add 2 FQuat structs
	FQuat AddTwoFQuatStructs(FQuat FirstFQuat , FQuat SecondFQuat);
	//This function is used to subtract 2 FQuat structs
	FQuat SubtractTwoFQuatStructs(FQuat FirstFQuat , FQuat SecondFQuat);
	//This fuction is used to move the camera around the player in left or right directions
	void MoveTheCameraLeftOrRight(float Direction);
	//This fuction is used to move and rotate the camera using the spring arm
	void ChangeCameraLocationAndRotation(FVector NewLocation , FRotator NewRotation);
	//Return the camera to it`s first transform
	void ResetCameraTransform();
	void PlayerMoveForwardOrBackward(float Direction);
	void PlayerMoveLeftOrRight(float Direction);
	void OpenMainMenu();
	//Every fighting style increase the melee weapon or the shooting weapon damage 
	void ApplySwordMasterStyle();
	void ApplyGunSlingerStyle();
	//This function is used to switch between the fighting styles
	void SwitchFightingStyle();
	//This function is used to do the sword attack combo
	void SwordNormalAttack();
	//This function will play the upper attack animation without going throw the 3 other attacks animations
	void SwordUpperAttack();
	//This function is called when the player want to aim on an enemy (To use the Aim function as a bind axis ,I have added the InputValue variable)
	void Aim(float InputValue);
	void DisableAim();
	void Shoot();
	//This function will be called by SetTimer to delay the shot gun shot
	void SetCanUseShotGunToTrue();
	//The next 3 function are used to switch between weapons
	void SwitchToSword();
	void SwitchToGun();
	void SwitchToShotGun();
};
