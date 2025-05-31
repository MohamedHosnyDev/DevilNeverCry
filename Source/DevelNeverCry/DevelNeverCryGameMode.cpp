// Copyright Epic Games, Inc. All Rights Reserved.

#include "DevelNeverCryGameMode.h"
#include "DevelNeverCryCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADevelNeverCryGameMode::ADevelNeverCryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
