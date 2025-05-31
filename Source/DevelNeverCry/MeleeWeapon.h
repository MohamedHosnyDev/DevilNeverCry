// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Particles/ParticleSystem.h"
#include "SwordCollisionBox.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class DEVELNEVERCRY_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	//I can not use the name RootComponent because it is already used in the AActor class
	USceneComponent * RootComp ;
	UPROPERTY(EditAnywhere,Category = "Weapon Static Mesh")
	UStaticMeshComponent * WeaponComponent ;
	UPROPERTY(EditAnywhere)
	USwordCollisionBox * SwordCollisionBox ;
	UPROPERTY(EditAnywhere,Category = "WeopenSettings")
	float DamageAmount = 20 ;
	UPROPERTY(EditAnywhere,Category = "WeopenSettings")
	float IncreaseDamageAmountFactor = 2 ;
	UPROPERTY(EditAnywhere,Category = "WeopenSettings")
	bool SpawnInTwoHands = false ;
	//We don`t want to damage the enemy or the player every frame for sure
	bool CanAttack ;
	//We will use IsActivatingCanAttack so that the compiler don`t call SetTimer function every frame
	bool IsActivatingCanAttack ;
	//This timer will help us to set CanAttack value
	FTimerHandle ActivateCanAttackTimer ;
	UPROPERTY(EditAnywhere,Category = "WeopenSettings")
	UParticleSystem * WeaponParticles ;
	UPROPERTY(EditAnywhere,Category = "WeopenSettings")
	USoundBase * WeopenSound ;
	//The weapon owner can be the player or the enemy
	AActor * WeaponOwner ;

	// Sets default values for this actor's properties
	AMeleeWeapon() ;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called by the OnHitComponent() function to cause damage to the enemy by the player or the player by the enemy
	void OnHitActor(AActor * HittedActor) ;
	//Called when the player activate the Sword Master fighting style
	void IncreaseDamageAmount() ;
	//Called when the player deactivate the Sword Master fighting style
	void DecreaseDamageAmount() ;
	//This function will be used by SetTimer() to set CanAttack to true
	void SetCanAttackToTrue();


};
