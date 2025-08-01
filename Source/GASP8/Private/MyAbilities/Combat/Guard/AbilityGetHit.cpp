// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAbilities/Combat/Guard/AbilityGetHit.h"

#include "MyTags/MyTags.h"
#include "MyAbilities/Combat/Guard/AbilityGuard.h"

UAbilityGetHit::UAbilityGetHit()
{
    this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    this->ShouldEnterCombat = true;
    FAbilityTriggerData triggerEvent;
    triggerEvent.TriggerSource = EGameplayAbilityTriggerSource::Type::GameplayEvent;
    triggerEvent.TriggerTag = Tags::Ability::get_hit;
    this->AbilityTriggers.Add(triggerEvent);
    this->DeflectHandle = nullptr;
    this->GuardHandle = nullptr;
}

void UAbilityGetHit::ActivateAbility(
    FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData)
{
    if (!TriggerEventData)
    {
        this->EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
        return;
    }
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    UAbilitySystemComponent *ownerASC = ActorInfo->AbilitySystemComponent.Get();
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("I GOT HIT"));
    this->EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UAbilityGetHit::OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);
}