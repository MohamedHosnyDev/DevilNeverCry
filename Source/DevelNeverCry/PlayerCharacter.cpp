// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "TheGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/MinElement.h"
#include "Components/PoseableMeshComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	
	SkeletalMesh = GetMesh();
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm -> SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> SetupAttachment(SpringArm);
	Tags.Add(TEXT("Player"));

	SwordAttackIndex = 0 ;
	GunIndex = 1 ;
	CurrentCollectedRedOrbsAmount = 0 ;

	HasChoosedAimingEnemy = false ;
	CanUseShotGun = true ;
	BoolVariables.IsDamaged = false ;
	BoolVariables.IsAttackingWithSword = false ;
	BoolVariables.IsAimingWithGun = false ;
	BoolVariables.IsAimingWithShotGun = false ;
	BoolVariables.IsDead = false ;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	/* We will use the GameInstance to get the player max health -
	because the player will be able to upgrade his max health level in the main menu */

	GameInstance = Cast<UTheGameInstance>(GetWorld() -> GetGameInstance()) ;
	MaxHealth = GameInstance->PlayerHealth ;
	CurrentHealth = MaxHealth ;

	SpringArmOldTransform = SpringArm -> GetRelativeTransform();

	/* When attempting to switch the fighting style before spawning each weapon at least once, 
		the game would crash. 

		To prevent this crash, I created every weapon as a static class (hidden by default). 
		This ensures that all weapons are initialized properly .

		So, I spawn every shooting weapon in the game begining and after that I destroy them . */
	RightGun = GetWorld()->SpawnActor<AShootingWeapon>(AShootingWeapon::StaticClass()) ;
	RightGun->Destroy();
	LeftGun = GetWorld()->SpawnActor<AShootingWeapon>(AShootingWeapon::StaticClass()) ;
	LeftGun -> Destroy();
	ShotGun = GetWorld()->SpawnActor<AShootingWeapon>(AShootingWeapon::StaticClass()) ;
	ShotGun -> Destroy () ;

	SwitchToSword();
	//Because the default fighting style is SwordMaster, I have to manually increase the damage the first time to prevent making the shooting weapons damage amount the half of it`s original value
	Sword -> IncreaseDamageAmount();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent -> BindAxis(TEXT("MoveTheCameraLeftOrRight"),this,&APlayerCharacter::MoveTheCameraLeftOrRight);
	
	PlayerInputComponent -> BindAxis(TEXT("MoveForward"),this,&APlayerCharacter::PlayerMoveForwardOrBackward);

	PlayerInputComponent -> BindAxis(TEXT("MoveBackward"),this,&APlayerCharacter::PlayerMoveForwardOrBackward);

	PlayerInputComponent -> BindAxis(TEXT("MoveRight"),this,&APlayerCharacter::PlayerMoveLeftOrRight);

	PlayerInputComponent -> BindAxis(TEXT("MoveLeft"),this,&APlayerCharacter::PlayerMoveLeftOrRight);

	PlayerInputComponent -> BindAxis(TEXT("Aim"),this,&APlayerCharacter::Aim);

	PlayerInputComponent -> BindAction(TEXT("DisableAim"),EInputEvent::IE_Released,this,&APlayerCharacter::DisableAim);

	PlayerInputComponent -> BindAction(TEXT("Shoot"),EInputEvent::IE_Pressed,this,&APlayerCharacter::Shoot);

	PlayerInputComponent -> BindAction(TEXT("NormalAttack"),EInputEvent::IE_Pressed,this,&APlayerCharacter::SwordNormalAttack);

	PlayerInputComponent -> BindAction(TEXT("UpperAttack"),EInputEvent::IE_Pressed,this,&APlayerCharacter::SwordUpperAttack);

	PlayerInputComponent -> BindAction(TEXT("SwitchWeaponToSword"),EInputEvent::IE_Pressed,this,&APlayerCharacter::SwitchToSword);

	PlayerInputComponent -> BindAction(TEXT("SwitchWeaponToGun"),EInputEvent::IE_Pressed,this,&APlayerCharacter::SwitchToGun);

	PlayerInputComponent -> BindAction(TEXT("SwitchWeaponToShotGun"),EInputEvent::IE_Pressed,this,&APlayerCharacter::SwitchToShotGun);

	PlayerInputComponent -> BindAction(TEXT("SwitchFightingStyle"),EInputEvent::IE_Pressed,this,&APlayerCharacter::SwitchFightingStyle);

	PlayerInputComponent -> BindAction(TEXT("ResetCamera"),EInputEvent::IE_Pressed,this,&APlayerCharacter::ResetCameraTransform);

	PlayerInputComponent -> BindAction(TEXT("GoToMainMenu"),EInputEvent::IE_Pressed,this,&APlayerCharacter::OpenMainMenu);

}


	int APlayerCharacter::GetSwordAttackIndex() const
	{
		return SwordAttackIndex ;
	}

	int APlayerCharacter::GetGunIndex() const
	{
		return GunIndex ;
	}

	void APlayerCharacter::ActivateDamageAnimationAndSound()
	{
		if(DamageSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld() , DamageSound , GetActorLocation());
		}
		
		/* While the player is attacking with the sword or switching fighting styles , 
		if he took a damage, a bug occurs where the attack animation or switching styles animations plays and gets stuck. 
		To fix this, we prevent the damage animation from activating (IsDamaged = true) while the player is attacking. */

		if(BoolVariables.IsAttackingWithSword || BoolVariables.IsSwitchingToGunSlingerStyle || BoolVariables.IsSwitchingToSwordMasterStyle)
		{return ;}
		BoolVariables.IsDamaged = true ;
		GetWorldTimerManager().SetTimer(AnimationTimerHandler,this,&APlayerCharacter::SetIsDamagedToFalse,0.5,false);
	}

	void APlayerCharacter::IncreasePlayerHealth()
	{
		float HealthIncreaseAmount = 20 ;
		CurrentHealth += HealthIncreaseAmount ;
	}

	void APlayerCharacter::IncreaseCollectedRedOrbsAmount()
	{
		int RedOrbsIncreaseAmount = 20 ;
		CurrentCollectedRedOrbsAmount += RedOrbsIncreaseAmount ;
	}

	float APlayerCharacter::GetPlayerHealthPercentage() const
	{
		return CurrentHealth / MaxHealth ;
	}

	int APlayerCharacter::GetExtraRedOrbs() const
	{
		ATheGameState * GameState = GetWorld()->GetGameState<ATheGameState>();
		return CurrentCollectedRedOrbsAmount - GameState->TargetRedOrbsNumber ;
	}

	int APlayerCharacter::GetCurrentCollectedRedOrbsAmount() const
	{
		return CurrentCollectedRedOrbsAmount ;
	}

	float APlayerCharacter::GetAimedAtEnemyHealthPercentage() const
	{
		if(NearestEnemy == nullptr)
		{
			return 0 ;
		}

		return NearestEnemy->CurrentHealth / NearestEnemy->MaxHealth ;
	}

	bool APlayerCharacter::GetIsDamaged() const
	{
		return BoolVariables.IsDamaged ;
	}

	void APlayerCharacter::SetIsDamagedToFalse()
	{
		BoolVariables.IsDamaged = false ;
	}

	bool APlayerCharacter::GetIsAttackingWithSword() const
	{
		return BoolVariables.IsAttackingWithSword ;
	}

	void APlayerCharacter::SetIsAttackingWithSwordToFalse()
	{
		BoolVariables.IsAttackingWithSword = false ;
	}

	bool APlayerCharacter::GetIsAimingWithGun() const
	{
		return BoolVariables.IsAimingWithGun ;
	}

	bool APlayerCharacter::GetIsAimingWithShotGun() const
	{
		return BoolVariables.IsAimingWithShotGun ;
	}

	FString APlayerCharacter::GetSpawnedWeaponName() const
	{
		return SpawnedWeaponName ;
	} 

	FString APlayerCharacter::GetFightingStyleName() const
	{
		if(Style == SwordMaster)
		{
			return TEXT("Sword master");
		}
		else
		{
			return TEXT("Gun slinger") ;
		}
	} 

	bool APlayerCharacter::GetIsSwitchingToGunSlingerStyle() const
	{
		return BoolVariables.IsSwitchingToGunSlingerStyle ;
	}

	void APlayerCharacter::SetIsSwitchingToGunSlingerStyleToFalse()
	{
		BoolVariables.IsSwitchingToGunSlingerStyle = false ;
	}

	bool APlayerCharacter::GetIsSwitchingToSwordMasterStyle() const
	{
		return BoolVariables.IsSwitchingToSwordMasterStyle ;
	}

	void APlayerCharacter::SetIsSwitchingToSwordMasterStyleToFalse()
	{
		BoolVariables.IsSwitchingToSwordMasterStyle = false ;
	}

	bool APlayerCharacter::GetIsDead()
	{
		if(CurrentHealth <= 0)
		{
			BoolVariables.IsDead = true ;
		}
		return BoolVariables.IsDead ;
	}


	FQuat APlayerCharacter::AddTwoFQuatStructs(FQuat FirstFQuat , FQuat SecondFQuat)
	{
		FQuat ResultFQuat = FQuat(FirstFQuat.X + SecondFQuat.X , FirstFQuat.Y + SecondFQuat.Y , FirstFQuat.Z + SecondFQuat.Z , FirstFQuat.W + SecondFQuat.W);
		return ResultFQuat ;
	}

	FQuat APlayerCharacter::SubtractTwoFQuatStructs(FQuat FirstFQuat , FQuat SecondFQuat)
	{
		FQuat ResultFQuat = FQuat(FirstFQuat.X - SecondFQuat.X , FirstFQuat.Y - SecondFQuat.Y , FirstFQuat.Z - SecondFQuat.Z , FirstFQuat.W - SecondFQuat.W);
		return ResultFQuat ;
	}

	void APlayerCharacter::MoveTheCameraLeftOrRight(float Direction)
	{
		if(BoolVariables.IsDead) 
		{return;}

		FRotator SpringArmRotation = SpringArm->GetRelativeRotation();
		SpringArm->SetRelativeRotation(SpringArmRotation + FRotator(0,Direction,0));
	}

	void APlayerCharacter::ChangeCameraLocationAndRotation(FVector NewLocation , FRotator NewRotation)
	{
		SpringArm -> SetRelativeLocation(NewLocation);
		SpringArm -> SetRelativeRotation(NewRotation);
	}

	void APlayerCharacter::ResetCameraTransform()
	{
		if(BoolVariables.IsDead)
		{return ;}

		FVector SpringArmOldLocation = SpringArmOldTransform.GetLocation();
		FQuat SpringArmOldRotationFQuat = SpringArmOldTransform.GetRotation();
		FRotator SpringArmOldRotation = FRotator(SpringArmOldRotationFQuat.X,SpringArmOldRotationFQuat.Y,SpringArmOldRotationFQuat.Z);

		ChangeCameraLocationAndRotation(SpringArmOldLocation,SpringArmOldRotation);
	}

	void APlayerCharacter::PlayerMoveForwardOrBackward(float Direction)
	{
		//We don`t want the player to move while attacking with the sword or doing the switching fighting style dance or if he is dead
		if(BoolVariables.IsAttackingWithSword || BoolVariables.IsSwitchingToGunSlingerStyle || BoolVariables.IsSwitchingToSwordMasterStyle || BoolVariables.IsDead)
		{
			return ;
		}

		//Based on the player components rotations that i assigned in the edittor , i have determined this SkeletalMesh rotation values because of a bug i faced

		if(!(BoolVariables.IsAimingWithGun || BoolVariables.IsAimingWithShotGun))
		{
			if(Direction == 1)
			{
				SkeletalMesh->SetRelativeRotation(SpringArm->GetRelativeRotation() + FRotator(0,-90,0));
			}
			else if(Direction == -1)
			{
				SkeletalMesh->SetRelativeRotation(SpringArm->GetRelativeRotation() + FRotator(0,90,0));
			}
			AddMovementInput(SpringArm->GetForwardVector() * Direction);
		}
		else
		{
			AddMovementInput(SkeletalMesh->GetRightVector() * Direction);
		}
	}

	void APlayerCharacter::PlayerMoveLeftOrRight(float Direction)
	{
		//We don`t want the player to move while attacking with the sword or doing the switching fighting style dance or he is dead
		if(BoolVariables.IsAttackingWithSword || BoolVariables.IsSwitchingToGunSlingerStyle || BoolVariables.IsSwitchingToSwordMasterStyle || BoolVariables.IsDead)
		{
			return ;
		}
		
		//Based on the player components rotations that i assigned in the edittor , i have determined this SkeletalMesh rotation values because of a bug i faced
		
		if(!(BoolVariables.IsAimingWithGun || BoolVariables.IsAimingWithShotGun))
		{
			if(Direction == 1)
			{
				SkeletalMesh->SetRelativeRotation(SpringArm->GetRelativeRotation() + FRotator(0,0,0));
			}
			else if(Direction == -1)
			{
				SkeletalMesh->SetRelativeRotation(SpringArm->GetRelativeRotation() + FRotator(0,-180,0));
			}
			AddMovementInput(SpringArm->GetRightVector() * Direction);
		}
		else
		{
			AddMovementInput(SkeletalMesh->GetForwardVector() * Direction * -1);
		}
	}

	void APlayerCharacter::OpenMainMenu()
	{
		UGameplayStatics::OpenLevel(GetWorld(),TEXT("MainMenu"));
	}

	void APlayerCharacter::ApplySwordMasterStyle()
	{
		RightGun -> DecreaseDamageAmount();
		LeftGun -> DecreaseDamageAmount();
		ShotGun -> DecreaseDamageAmount();
		Sword -> IncreaseDamageAmount();
		Style = SwordMaster ;
	}

	void APlayerCharacter::ApplyGunSlingerStyle()
	{
		Sword -> DecreaseDamageAmount();
		RightGun -> IncreaseDamageAmount();
		LeftGun -> IncreaseDamageAmount();
		ShotGun -> IncreaseDamageAmount();
		Style = GunSlinger ;
	}

	void APlayerCharacter::SwitchFightingStyle()
	{
		//We dont want the player to be able to switch fighting styles if he is dead
		
		/* If the player switched his fighting style during attacking with the sword the animation will freeze 
		so to solve this bug the player must not be able to switch the fighting style while he is attacking with the sword */
		if(BoolVariables.IsDead || BoolVariables.IsAttackingWithSword)
		{return ;}

		if(Style == SwordMaster)
		{
			if(SwitchToGunSlingerStyleSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),SwitchToGunSlingerStyleSound,GetActorLocation());
			}
			Style = GunSlinger ;
			ApplyGunSlingerStyle();
			BoolVariables.IsSwitchingToGunSlingerStyle = true ;
			GetWorldTimerManager().SetTimer(AnimationTimerHandler,this,&APlayerCharacter::SetIsSwitchingToGunSlingerStyleToFalse,0.8,false);
		}
		else
		{
			if(SwitchToSwordMasterStyleSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),SwitchToSwordMasterStyleSound,GetActorLocation());
			}
			Style = SwordMaster ;
			ApplySwordMasterStyle();
			BoolVariables.IsSwitchingToSwordMasterStyle = true ;
			GetWorldTimerManager().SetTimer(AnimationTimerHandler,this,&APlayerCharacter::SetIsSwitchingToSwordMasterStyleToFalse,1,false);
		}
	}

	void APlayerCharacter::SwordNormalAttack()
	{
		//If the player is not using the sword or is already attacking or is dead the function will not do any thing
		if(BoolVariables.IsAttackingWithSword || BoolVariables.IsDamaged || SpawnedWeaponName != TEXT("Sword") || BoolVariables.IsDead)
		{return ;}

		if(SwordAttackIndex == 4)
		{
			SwordAttackIndex = 1 ;
		}
		else
		{
			SwordAttackIndex ++ ;
		}

		BoolVariables.IsAttackingWithSword = true ;
		GetWorldTimerManager().SetTimer(AnimationTimerHandler,this,&APlayerCharacter::SetIsAttackingWithSwordToFalse,0.5,false);
	}

	void APlayerCharacter::SwordUpperAttack()
	{
		//If the player is not using the sword or is already attacking or is dead the function will not do any thing
		if(BoolVariables.IsAttackingWithSword || BoolVariables.IsDamaged ||SpawnedWeaponName != TEXT("Sword") || BoolVariables.IsDead)
		{return ;}

		//The upper attack animation index is 4
		SwordAttackIndex = 4 ;
		BoolVariables.IsAttackingWithSword = true ;
		GetWorldTimerManager().SetTimer(AnimationTimerHandler,this,&APlayerCharacter::SetIsAttackingWithSwordToFalse,0.6,false);
	}

	void APlayerCharacter::Aim(float InputValue)
	{
		//The player can`t aim wth a sword or if he is dead
		
		if(SpawnedWeaponName == TEXT("Sword") || BoolVariables.IsDead || InputValue != 1)
		{return ;}

		else if(SpawnedWeaponName == TEXT("Gun"))
		{
			BoolVariables.IsAimingWithGun = true ;
		}

		else if(SpawnedWeaponName == TEXT("ShotGun"))
		{
			BoolVariables.IsAimingWithShotGun = true ;
		}

		if(!HasChoosedAimingEnemy)
		{
			TArray<AActor*> Enemies ;
			float ShortestDistance ;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(),AEnemy::StaticClass(),Enemies);

			for(AActor * Actor : Enemies)
			{
				//We don`t want to aim to a dead enemy so we will remove him from the array

				NearestEnemy = Cast<AEnemy>(Actor);
				if(NearestEnemy->IsDead)
				{
					Enemies.Remove(Actor);
				}
				NearestEnemy = nullptr ;
			}

			if(Enemies.Num() == 0)
			{
				return ;
			}

			NearestEnemy = Cast<AEnemy>(UGameplayStatics::FindNearestActor(GetActorLocation() , Enemies , ShortestDistance)) ;
			if(NearestEnemy == nullptr)
			{
				return ;
			}

			NearestEnemy -> ShowHealthWidget();
			HasChoosedAimingEnemy = true ;
		 }

		if(NearestEnemy->IsDead)
		{
			NearestEnemy -> HideHealthWidget();
			HasChoosedAimingEnemy = false ;
			return ;
		}

		//We will change the spring arm relative location and relative rotation while aiming
		FVector SpringArmTargetLocation = FVector(55,235,135);
		FVector OffsetVectorFromCameraToEnemy = NearestEnemy->GetActorLocation() - SpringArm->GetComponentLocation() ;
		FRotator SpringArmTargetRotation = OffsetVectorFromCameraToEnemy.Rotation();
		ChangeCameraLocationAndRotation(SpringArmTargetLocation , SpringArmTargetRotation);

		FVector OffsetVectorFromPlayerToEnemy = NearestEnemy->GetActorLocation() - GetActorLocation() ;
		//This -90 degrees are the skelatel mesh original yaw rotation
		FRotator PlayerRotator = FRotator(GetActorRotation().Pitch, OffsetVectorFromPlayerToEnemy.Rotation().Yaw - 90 ,GetActorRotation().Roll) ;
		SkeletalMesh->SetRelativeRotation(PlayerRotator);

	}

	void APlayerCharacter::DisableAim()
	{
		//The player can`t aim from the begining with a sword or if he is dead

		if(SpawnedWeaponName == TEXT("Sword") || BoolVariables.IsDead)
		{return ;}

		else if(BoolVariables.IsAimingWithGun)
		{
			BoolVariables.IsAimingWithGun = false ;
		}

		else if(BoolVariables.IsAimingWithShotGun)
		{
			BoolVariables.IsAimingWithShotGun = false ;
		}

		HasChoosedAimingEnemy = false ;

		ResetCameraTransform();

		if(NearestEnemy == nullptr)
		{	return;}
		NearestEnemy -> HideHealthWidget();
	}

	void APlayerCharacter::Shoot()
	{
		//The player will not be able to shoot if he is using the sword or is being damaged now or is not aiming with a shooting weapon
		if(SpawnedWeaponName == TEXT("Sword") || BoolVariables.IsDamaged || !(BoolVariables.IsAimingWithGun || BoolVariables.IsAimingWithShotGun))
		{return ;}

		else if(SpawnedWeaponName == TEXT("Gun"))
		{
			switch (GunIndex)
			{
			case 1:
				RightGun -> Shoot();
				GunIndex ++ ;
				break;
			case 2:
				LeftGun -> Shoot();
				GunIndex = 1 ;
				break;
			}
		}

		else if (SpawnedWeaponName == TEXT("ShotGun") && CanUseShotGun)
		{
			//We use only the RightGun because the shot gun is spawned in only the right hand
			ShotGun -> Shoot();
			CanUseShotGun = false ;
			GetWorldTimerManager().SetTimer(ShotGunTimerHandler,this,&APlayerCharacter::SetCanUseShotGunToTrue,0.9,false);
		}
		
	}

	void APlayerCharacter::SetCanUseShotGunToTrue()
	{
		CanUseShotGun = true ;
	}

	void APlayerCharacter::SwitchToSword()
	{
		//If the sword is already spawned or the player is dead the function will not do any thing
		//The player cannot switch to the sword while aiming because the camera and movement controls will conflict, leading to unintended behavior
		if(SpawnedWeaponName == TEXT("Sword") || BoolVariables.IsAimingWithGun || BoolVariables.IsAimingWithShotGun || BoolVariables.IsDead)
		{return ;}

		if(RightSpawnedWeapon)
		{
			RightSpawnedWeapon -> Destroy() ;
		}

		//Only the gun is spawned in the left hand too
		if(LeftSpawnedWeapon)
		{
			LeftSpawnedWeapon -> Destroy() ;
		}

		if(SwitchWeaponSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld() , SwitchWeaponSound , GetActorLocation());
		}

		Sword = GetWorld()->SpawnActor<AMeleeWeapon>(TheSword);
		Sword -> SetOwner(this);
		Sword -> AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("RightWeaponPlace"));

		RightSpawnedWeapon = Sword ;
		LeftSpawnedWeapon = nullptr ;
		SpawnedWeaponName = TEXT("Sword") ;
	}

	void APlayerCharacter::SwitchToGun()
	{
		//If the Guns are already spawned or the player is attacking with the sword or the player is aiming with shot gun or the player is dead ,The function will not do any thing
		if(SpawnedWeaponName == TEXT("Gun") || BoolVariables.IsAttackingWithSword || BoolVariables.IsAimingWithShotGun || BoolVariables.IsDead)
		{return ;}

		RightSpawnedWeapon -> Destroy() ;

		if(SwitchWeaponSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld() , SwitchWeaponSound , GetActorLocation());
		}

		RightGun = GetWorld()->SpawnActor<AShootingWeapon>(TheGun);
		RightGun -> AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("RightWeaponPlace"));

		LeftGun = GetWorld()->SpawnActor<AShootingWeapon>(TheGun);
		LeftGun -> AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("LeftWeaponPlace"));

		RightSpawnedWeapon = RightGun ;
		LeftSpawnedWeapon = LeftGun ;
		SpawnedWeaponName = TEXT("Gun") ;
	}

	void APlayerCharacter::SwitchToShotGun()
	{
		//If the Shot Gun is already spawned or the player is attacking with the sword or the player is aiming with guns or the player is dead ,The function will not do any thing
		if(SpawnedWeaponName == TEXT("ShotGun") || BoolVariables.IsAttackingWithSword || BoolVariables.IsAimingWithGun || BoolVariables.IsDead)
		{return ;}

		RightSpawnedWeapon -> Destroy() ;

		if(LeftSpawnedWeapon)
		{
			LeftSpawnedWeapon -> Destroy() ;
		}

		if(SwitchWeaponSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld() , SwitchWeaponSound , GetActorLocation());
		}
		
		ShotGun = GetWorld()->SpawnActor<AShootingWeapon>(TheShotGun);
		ShotGun -> AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("RightWeaponPlace"));

		RightSpawnedWeapon = ShotGun ;
		LeftSpawnedWeapon = nullptr ;
		SpawnedWeaponName = TEXT("ShotGun") ;
	}