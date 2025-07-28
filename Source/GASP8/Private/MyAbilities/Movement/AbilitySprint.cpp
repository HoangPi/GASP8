// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAbilities/Movement/AbilitySprint.h"

#include "MyEffects/Attribute/Stamina/EffectDrainStamina.h"
#include "MyTags/MyTags.h"

UAbilitySprint::UAbilitySprint()
{
    this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
    this->ShouldEnterCombat = true;
}

void UAbilitySprint::ActivateAbility(
    FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    const float level = 1.0f;
    this->EffectSpecDrainStamina = this->MakeOutgoingGameplayEffectSpec(UEffectDrainStamina::StaticClass(), level);
    this->EffectSpecDrainStamina.Data.Get()->SetSetByCallerMagnitude(Tags::Attribute::stamina, level * -5.0f);

    ((ACharacter *)ActorInfo->OwnerActor.Get())->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    this->EffectHandleDrainStamina = ActorInfo->AbilitySystemComponent.Get()->ApplyGameplayEffectSpecToSelf(*this->EffectSpecDrainStamina.Data.Get());
}

void UAbilitySprint::EndAbility(
    FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    ((ACharacter *)ActorInfo->OwnerActor.Get())->GetCharacterMovement()->MaxWalkSpeed = 230.0f;
    ActorInfo->AbilitySystemComponent.Get()->RemoveActiveGameplayEffect(this->EffectHandleDrainStamina, 1);
}