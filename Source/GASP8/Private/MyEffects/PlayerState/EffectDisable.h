// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"

#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameplayEffectComponents/AdditionalEffectsGameplayEffectComponent.h"

#include "EffectDisable.generated.h"

/**
 * 
 */
UCLASS()
class UEffectDisable : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UEffectDisable();
};
