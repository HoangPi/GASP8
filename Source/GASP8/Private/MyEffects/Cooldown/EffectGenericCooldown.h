// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "EffectGenericCooldown.generated.h"

/**
 * 
 */
UCLASS()
class UEffectGenericCooldown : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UEffectGenericCooldown();

	// Set the new cooldown for sub object (the default cooldown is 5s)
	void SetCooldown(float NewCooldown);
};
