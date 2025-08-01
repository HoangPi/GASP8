// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAbilities/Combat/Guard/AbilityGetHit.h"

#include "MyTags/MyTags.h"
#include "MyAbilities/Combat/Guard/AbilityGuard.h"
#include "MyEffects/Attribute/Health/EffectReduceHealth.h"

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
    if (ownerASC->GetActiveGameplayEffect(*this->DeflectHandle))
    {
        ownerASC->RemoveActiveGameplayEffect(*this->DeflectHandle, 1);
    }
    else if (ownerASC->GetActiveGameplayEffect(*this->GuardHandle))
    {
        ownerASC->RemoveActiveGameplayEffect(*this->GuardHandle, 1);
    }
    else if (ownerASC->HasMatchingGameplayTag(Tags::PlayerState::manual_guard))
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString("Dull block"));
    }
    else
    {
        FGameplayEffectSpec spec = FGameplayEffectSpec(
            (UEffectReduceHealth *)UEffectReduceHealth::StaticClass()->GetDefaultObject(),
            ownerASC->MakeEffectContext());
        spec.SetSetByCallerMagnitude(Tags::Attribute::health, -TriggerEventData->EventMagnitude);
        ownerASC->ApplyGameplayEffectSpecToSelf(spec);
    }
    this->EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UAbilityGetHit::OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);
    UAbilityGuard *guard = (UAbilityGuard *)ActorInfo->AbilitySystemComponent.Get()->FindAbilitySpecFromClass(UAbilityGuard::StaticClass())->GetPrimaryInstance();
    this->DeflectHandle = &guard->DeflectHandle;
    this->GuardHandle = &guard->GuardHandle;
}