// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"



// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget -> SetupAttachment(RootComponent);
	//We will check this tag to damage the enemy using a melee weapon
	Tags.Add(TEXT("Enemy"));

	IsReviving = IsMoving = IsAttacking = IsNormallyDamaged = IsUpperDamaged = IsDead = IsStandingUp = false ;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth ;
	EnemyOldZAxisPositionValue = GetActorLocation().Z ;

	SpawnWeapon();
	HideHealthWidget();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetActorLocation().Z <= EnemyOldZAxisPositionValue && IsUpperDamaged)
	{
		IsUpperDamaged = false ;
	}

	SpawnRedOrbsAndReviveEnemy();
}

void AEnemy::SpawnRedOrbsAndReviveEnemy()
{
	if(GetIsDead() && !IsReviving)
	{
		IsReviving = true ;
		int NumberOfSpawnedRedOrbs = 0 ;

		while(NumberOfSpawnedRedOrbs != 4)
		{
			GetWorld()->SpawnActor<AActor>(RedOrbActor , GetActorLocation() + FVector(30,0,100) , GetActorRotation());
			NumberOfSpawnedRedOrbs ++ ;
		}

		GetWorldTimerManager().SetTimer(ReviveTimer,this,&AEnemy::Revive,30,false);
	}
}

void AEnemy::SpawnWeapon()
{
	if(TheMeleeWeapon == nullptr)
	{return ;}
	RightMeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(TheMeleeWeapon);
	RightMeleeWeapon -> SetOwner(this);
	RightMeleeWeapon -> AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("RightWeaponPlace"));
	if(RightMeleeWeapon->SpawnInTwoHands)
	{
		LeftMeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(TheMeleeWeapon);
		LeftMeleeWeapon -> AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("LeftWeaponPlace"));
		LeftMeleeWeapon -> SetOwner(this);
	}
}

void AEnemy::Revive()
{
	CurrentHealth = MaxHealth ;
	IsDead = false ;
	IsReviving = false ;
}

void AEnemy::ActivateDamageAnimation(int SwordAttackIndex)
{
	if(SwordAttackIndex == 4)
	{
		IsUpperDamaged = true ;
	}
	else
	{
		IsNormallyDamaged = true ;
		GetWorldTimerManager().SetTimer(DamageTimer,this,&AEnemy::SetIsDamagedToFalse,0.5,false);
	}
}

void AEnemy::ShowHealthWidget()
{
	if(HealthWidget -> GetWidget())
	{
		HealthWidget -> SetVisibility(true);
	}
}

void AEnemy::HideHealthWidget()
{
	if(HealthWidget -> GetWidget())
	{
		HealthWidget -> SetVisibility(false);
	}
}

float AEnemy::GetHealthPercentage() const 
{
	return CurrentHealth/MaxHealth ;
}

bool AEnemy::GetIsMoving() const 
{
	return IsMoving ;
}

void AEnemy::SetIsMovingTrue()
{
	IsMoving = true ;
}

bool AEnemy::GetIsAttacking() const 
{
	return IsAttacking ;
}

bool AEnemy::GetIsNormallyDamaged() const 
{
	return IsNormallyDamaged ; 
}

bool AEnemy::GetIsUpperDamaged() const 
{
	return IsUpperDamaged ; 
}

void AEnemy::SetIsDamagedToFalse()
{
	IsNormallyDamaged = false ;
	IsUpperDamaged = false ;
}

bool AEnemy::GetIsDead()
{
	if(CurrentHealth <= 0)
	{
		IsDead = true ;
	}
	return IsDead ;
}

