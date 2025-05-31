// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordCollisionBox.h"


AActor * USwordCollisionBox::GetOverlappingCharacter()
{
    GetOverlappingActors(Actors) ;
    if(Actors.Num() == 0)
    {
        return nullptr ;
    }
    return Actors[0] ;
}