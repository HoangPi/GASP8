// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicalAssets/Abilities/AbilityBase.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySprint.generated.h"

/**
 * 
 */
UCLASS()
class UAbilitySprint : public UAbilityBase
{
	GENERATED_BODY()

public:
	UAbilitySprint();

	FActiveGameplayEffectHandle EffectHandleDrainStamina;
	FGameplayEffectSpecHandle EffectSpecDrainStamina;

	void ActivateAbility(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData);

	virtual void EndAbility(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled);
};
