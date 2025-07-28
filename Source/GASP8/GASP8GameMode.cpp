// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASP8GameMode.h"
#include "GASP8Character.h"
#include "UObject/ConstructorHelpers.h"

AGASP8GameMode::AGASP8GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
