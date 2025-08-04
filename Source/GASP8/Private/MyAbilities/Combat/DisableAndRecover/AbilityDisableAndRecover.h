// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectRemoved.h"

#include "AbilityDisableAndRecover.generated.h"

/**
 *
 */
UCLASS()
class UAbilityDisableAndRecover : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAbilityDisableAndRecover();

	bool CanRecover;

	UAnimMontage *FallDownAndRecoverMontage;
	UAnimMontage *StandUpMontage;

	FScriptDelegate OnMontageCompletedDelegate;
	FScriptDelegate OnRecieveNotifyDelegate;
	FScriptDelegate OnMontageInteruptedDelegate;
	FScriptDelegate OnDisableEnd;

	UFUNCTION()
	void NotifyMontageCompleted();

	UFUNCTION()
	void NotifyDisableEffectEnded();

	void ActivateAbility(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData);

	void CancelAbility(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility);
};
