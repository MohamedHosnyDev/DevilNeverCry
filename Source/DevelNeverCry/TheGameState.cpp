// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGameState.h"
#include "Kismet/GameplayStatics.h"



ATheGameState::ATheGameState()
{
    PrimaryActorTick.bCanEverTick = true ;

    GameEnded = false ;
}

void ATheGameState::BeginPlay()
{
    RemainingTime = TargetMaxTime ;
    
    PlayerController = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController()) ;
    Player = Cast<APlayerCharacter>(PlayerController->GetPawn());
    GameInstance = Cast<UTheGameInstance>(GetWorld()->GetGameInstance());

    GetWorldTimerManager().SetTimer(DecreaseTimeHandler,this,&ATheGameState::DecreaseTime,1,true);
}
		
void ATheGameState::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

    if(TheWinWidget == nullptr || TheLoseWidget == nullptr || GameEnded)
    {   return ;}

    CheckWinAndLoseConditions();

}

int ATheGameState::GetTargetRedOrbsNumber() const 
{
    return TargetRedOrbsNumber ;
}

int ATheGameState::GetRemainingTimeMinutes() const 
{
    int result = (RemainingTime - (RemainingTime % 60)) / 60 ;
    return result ;
}

int ATheGameState::GetRemainingTimeSeconds() const 
{
    int result = RemainingTime % 60 ;
    return result ;
}

void ATheGameState::CheckWinAndLoseConditions()
{
    if(Player->BoolVariables.IsDead)
    {
        PlayerController->RemoveGameplayWidget();
        PlayerController->GameHasEnded(Player , false);
        PlayLoseWidget();
        GameEnded = true ;
    }
    else if (RemainingTime == 0)
    {
        if(Player->CurrentCollectedRedOrbsAmount >= TargetRedOrbsNumber)
        {
            PlayerController->RemoveGameplayWidget();
            PlayerController->GameHasEnded(Player , true);
            GameInstance->ExtraRedOrbsAmount += Player->GetExtraRedOrbs();
            PlayWinWidget();
        }
        else
        {
            PlayerController->RemoveGameplayWidget();
            PlayerController->GameHasEnded(Player , true);
            PlayLoseWidget();
        }

        GameEnded = true ;
    }
}

void ATheGameState::PlayWinWidget()
{
    if(TheWinWidget == nullptr)
    {return ;}

    WinWidget = CreateWidget(GetWorld(),TheWinWidget);
    WinWidget -> AddToViewport() ;
}

void ATheGameState::PlayLoseWidget()
{
    if(TheLoseWidget == nullptr)
    {return ;}
    
    LoseWidget = CreateWidget(GetWorld(),TheLoseWidget);
    LoseWidget -> AddToViewport() ;
}

void ATheGameState::DecreaseTime()
{
    RemainingTime--;
}

