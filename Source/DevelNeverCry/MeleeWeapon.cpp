// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
//I included this Enemy and Player character header files here to avoid circular dependency
#include "Enemy.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AEnemy * Enemy ;
APlayerCharacter * Player ;

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp) ;
	WeaponComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComponent"));
	WeaponComponent->SetupAttachment(RootComp);
	SwordCollisionBox = CreateDefaultSubobject<USwordCollisionBox>(TEXT("SwordCollisionBox"));
	SwordCollisionBox->SetupAttachment(WeaponComponent);
	CanAttack = true ;
	IsActivatingCanAttack = false ;
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(SwordCollisionBox -> GetOverlappingCharacter())
	{
		OnHitActor(SwordCollisionBox -> GetOverlappingCharacter()) ;
	}
}

void AMeleeWeapon::OnHitActor(AActor * HittedActor)
{
	if(GetOwner() == nullptr)
	{return ;}
	
	WeaponOwner = GetOwner();

	if(HittedActor == WeaponOwner || ! CanAttack)
	{
		if(! CanAttack && ! IsActivatingCanAttack) 
		{
			GetWorldTimerManager().SetTimer(ActivateCanAttackTimer,this,&AMeleeWeapon::SetCanAttackToTrue,0.4,false);
			IsActivatingCanAttack = true ;
		}
		return ; 
	}
	
	bool IsPlayer = WeaponOwner -> ActorHasTag("Player") ;

	switch (IsPlayer)
	{
	case true :
		/* We want to damage the enemy only when the player is attacking him And to know this we have to access -
		his IsAttackingWithSword boolean variable (to do that we have to cast the WeaponOwner)*/

		Player = Cast<APlayerCharacter>(WeaponOwner) ;
		if(HittedActor -> ActorHasTag("Enemy") && Player -> BoolVariables.IsAttackingWithSword)
		{
			if(WeaponParticles && HittedActor != WeaponOwner)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),WeaponParticles,GetActorLocation(),GetActorRotation());
			}

			if(WeopenSound && HittedActor != WeaponOwner)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),WeopenSound,GetActorLocation(),GetActorRotation());
			}

			Enemy = Cast<AEnemy>(HittedActor) ;
			Enemy -> CurrentHealth -= DamageAmount ;
			//The sword hit will make the enemy step back slightly (I have rotated the enemy mesh 90 degrees to the right )
			Enemy->SetActorLocation(Enemy->GetActorLocation() - (Enemy->GetActorRightVector() * 5));
			Enemy->ActivateDamageAnimation(Player->SwordAttackIndex);
			//If the sword attack index == 4 the enemy will fly in the air
			if(Player->SwordAttackIndex == 4)
			{
				FVector EnemyOldLocation = Enemy->GetActorLocation();
				float EnemyDistanceDifference = 0;
				FVector EnemyCurrentLocation = Enemy -> GetActorLocation();
				FVector EnemyTargetLocation = EnemyCurrentLocation + FVector(0,0,4000);
				float MoveSpeed = 2 ;
				float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
				FVector EnemyNewLocation = FMath::VInterpTo(EnemyCurrentLocation , EnemyTargetLocation , MoveSpeed , DeltaTime);
				Enemy->SetActorLocation(EnemyNewLocation);
			}

			CanAttack = false ;
		}
		break;
	
	case false :

		Enemy = Cast<AEnemy>(WeaponOwner) ;

		if(HittedActor -> ActorHasTag("Player") && !(Enemy->IsNormallyDamaged || Enemy->IsUpperDamaged || Enemy->IsDead || Enemy->IsMoving ))
		{
			if(WeaponParticles && HittedActor != WeaponOwner)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),WeaponParticles,GetActorLocation(),GetActorRotation());
			}

			if(WeopenSound && HittedActor != WeaponOwner)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),WeopenSound,GetActorLocation(),GetActorRotation());
			}

			Player = Cast<APlayerCharacter>(HittedActor);
			Player -> CurrentHealth -= DamageAmount ;
			Player->ActivateDamageAnimationAndSound();

			CanAttack = false ;
		}
		break;
	}
}

void AMeleeWeapon::IncreaseDamageAmount()
{
	DamageAmount = DamageAmount * IncreaseDamageAmountFactor ;
}

void AMeleeWeapon::DecreaseDamageAmount()
{
	DamageAmount = DamageAmount / IncreaseDamageAmountFactor ;
}

void AMeleeWeapon::SetCanAttackToTrue()
{
	CanAttack = true ;
	IsActivatingCanAttack = false ;
}