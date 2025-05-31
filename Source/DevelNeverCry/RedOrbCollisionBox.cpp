// Fill out your copyright notice in the Description page of Project Settings.


#include "RedOrbCollisionBox.h"
#include "Kismet/GameplayStatics.h"


URedOrbCollisionBox::URedOrbCollisionBox()
{
    PrimaryComponentTick.bCanEverTick = true ;
}

void URedOrbCollisionBox::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
                
                Player -> IncreaseCollectedRedOrbsAmount();

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