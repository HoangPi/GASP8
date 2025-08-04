// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicalAssets/Abilities/AbilityBase.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimSequence.h"

#include "AbilityGetHit.generated.h"

/**
 *
 */
UCLASS()
class UAbilityGetHit : public UAbilityBase
{
	GENERATED_BODY()

public:
	UAbilityGetHit();

	FActiveGameplayEffectHandle *DeflectHandle;
	FActiveGameplayEffectHandle *GuardHandle;
	UAnimMontage *FallDownMontage;
	FScriptDelegate OnFallDownCompleteDelegate;

	UFUNCTION()
	void OnFallDownComplete();

	inline void HandleDisablePlayer(UAbilitySystemComponent *ownerASC, FGameplayEffectContextHandle &context);

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec);

	void ActivateAbility(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData);
};
