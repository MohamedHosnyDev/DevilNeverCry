// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "Particles/ParticleSystem.h"
#include "ShootingWeapon.generated.h"

UCLASS()
class DEVELNEVERCRY_API AShootingWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	//I can not use the name RootComponent because it is already used in the AActor class
	USceneComponent * RootComp ;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent * WeaponComponent ;
	UPROPERTY(EditAnywhere)
	USceneComponent * ShootingComponent ;
	//Used to know if the weapon is used as a gun or a shot gun because there are some differences in the damage amount and the number of spawned weapons
	UPROPERTY(EditAnywhere , Category = "WeaponSettings")
	bool IsGun = true ;
	UPROPERTY(EditDefaultsOnly , Category = "WeaponSettings")
	float DamageAmount = 5 ;
	UPROPERTY(EditDefaultsOnly , Category = "WeaponSettings")
	float IncreaseDamageAmountFactor = 2 ;
	//Used to determine the shot gun real damage amount depanding on how far the target is from the player
	float ShotGunDamageAmount ;
	AEnemy * Enemy ;
	//Enemy location and enemy distance is used to calculate the shot gun real damage amount
	FVector EnemyLocation ;
	float EnemyDistance ;
	
	FVector BulletShootingLocation ;
	FHitResult HitResult ;
	//trace is used to determine if we shooted an enemy or not
	bool trace ;

	UPROPERTY(EditDefaultsOnly , Category = "WeaponParicles")
	UParticleSystem * ShootingParticles ;
	UPROPERTY(EditDefaultsOnly , Category = "WeaponParicles")
	UParticleSystem * ImpactParticles ;
	UPROPERTY(EditDefaultsOnly , Category = "WeaponSound")
	USoundBase * ShootingSound ;

	AShootingWeapon();
	//Used to get the shooted actor
	void Shoot() ;
	//This two functions are used when activating or deactivating GunSlinger Fighting style
	void IncreaseDamageAmount() ;
	void DecreaseDamageAmount() ;


};
