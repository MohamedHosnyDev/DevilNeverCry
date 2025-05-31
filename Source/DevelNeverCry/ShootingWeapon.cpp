// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AShootingWeapon::AShootingWeapon()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);
	WeaponComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComponent"));
	WeaponComponent->SetupAttachment(RootComp);
	ShootingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ShootingComponent"));
	ShootingComponent->SetupAttachment(WeaponComponent);
}

void AShootingWeapon :: Shoot() 
{
	FVector BulletRange ;
	
	if(ShootingParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ShootingParticles,GetActorLocation());
	}
	if(ShootingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),ShootingSound,GetActorLocation());
	}

	if(IsGun)
	{
		BulletShootingLocation = ShootingComponent->GetComponentLocation();
		BulletRange = GetActorLocation() + GetActorRotation().Vector()*4000 + FVector(0,0,30);
		trace = GetWorld()->LineTraceSingleByChannel(HitResult,BulletShootingLocation,BulletRange,ECC_GameTraceChannel1);
		
		if(trace)
		{
			Enemy = Cast<AEnemy>(HitResult.GetActor());
			Enemy->CurrentHealth -= DamageAmount ;
			Enemy->ActivateDamageAnimation(0);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticles,Enemy->GetActorLocation());
		}
		
	}
	else
	{
		BulletShootingLocation = ShootingComponent->GetComponentLocation();
		BulletRange = GetActorLocation() + GetActorRotation().Vector()*1000 + FVector(0,0,30);
		trace = GetWorld()->LineTraceSingleByChannel(HitResult,BulletShootingLocation,BulletRange,ECC_GameTraceChannel1);

		if(trace)
		{
			Enemy = Cast<AEnemy>(HitResult.GetActor());

			EnemyLocation = Enemy->GetActorLocation();
			EnemyDistance = FVector::Dist(GetActorLocation(),EnemyLocation);
			ShotGunDamageAmount = ((1000 - EnemyDistance) *0.001)*DamageAmount ;
			
			Enemy->CurrentHealth -= ShotGunDamageAmount ;
			Enemy->ActivateDamageAnimation(0);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticles,Enemy->GetActorLocation());
		}

	}
}

void AShootingWeapon :: IncreaseDamageAmount() 
{
	DamageAmount = DamageAmount * IncreaseDamageAmountFactor ;
}

void AShootingWeapon :: DecreaseDamageAmount() 
{
	DamageAmount = DamageAmount / IncreaseDamageAmountFactor ;
}
