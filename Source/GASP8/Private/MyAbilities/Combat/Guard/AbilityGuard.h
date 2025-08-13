// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicalAssets/Abilities/AbilityBase.h"
#include "AbilityGuard.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerGuard, bool, IsGuarding);

UCLASS()
class UAbilityGuard : public UAbilityBase
{
	GENERATED_BODY()

public:
	UAbilityGuard();

	FActiveGameplayEffectHandle DeflectHandle;
	FActiveGameplayEffectHandle GuardHandle;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerGuard NotifyPlayerGuard;

	void ActivateAbility(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData);

	void EndAbility(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled);
};
