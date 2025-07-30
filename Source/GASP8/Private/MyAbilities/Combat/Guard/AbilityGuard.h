// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicalAssets/Abilities/AbilityBase.h"
#include "AbilityGuard.generated.h"

/**
 * 
 */
UCLASS()
class UAbilityGuard : public UAbilityBase
{
	GENERATED_BODY()

public:
	UAbilityGuard();

	FActiveGameplayEffectHandle DeflectHandle;
	FActiveGameplayEffectHandle GuardHandle;

	void ActivateAbility(
		FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo *ActorInfo, 
		FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData *TriggerEventData);

};
