// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenOrbCollisionBox.h"
#include "Kismet/GameplayStatics.h"


UGreenOrbCollisionBox::UGreenOrbCollisionBox()
{
    PrimaryComponentTick.bCanEverTick = true ;
}

void UGreenOrbCollisionBox::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime , TickType , ThisTickFunction);
    
    GetOverlappingActors(OverlappingActors);
    
    if(OverlappingActors.Num() != 0)
    {
        for(AActor * Actor : OverlappingActors)
        {
            if(Actor -> ActorHasTag("Player"))
            {
                Player = Cast<APlayerCharacter>(Actor);
                
                if(Player->MaxHealth == Player->CurrentHealth)
                {
                    return ;
                }
                else if (Player->MaxHealth - Player->CurrentHealth <= 20)
                {
                    Player->CurrentHealth = Player->MaxHealth ;
                }
                else
                {
                    Player -> IncreasePlayerHealth();
                }
                
                if(CollectSound)
                {
                    UGameplayStatics::PlaySoundAtLocation(GetWorld() , CollectSound , GetComponentLocation());
                }
                if(CollectParticles)
                {
                    UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , CollectParticles , GetComponentLocation());
                }
                
                GetOwner()->Destroy();
            }
        }
    }
}