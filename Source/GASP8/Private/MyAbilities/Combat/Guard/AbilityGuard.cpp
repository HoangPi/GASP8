// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAbilities/Combat/Guard/AbilityGuard.h"

#include "MyEffects/PlayerState/EffectOutOfCombat.h"
#include "MyTags/MyTags.h"
#include "MyEffects/Combat/Guard/EffectDeflect.h"
#include "MyEffects/Combat/Guard/EffectGuard.h"
#include "MyTags/MyTags.h"

UAbilityGuard::UAbilityGuard()
{
    this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    this->ShouldEnterCombat = false;
    this->ActivationOwnedTags.AddTag(Tags::PlayerState::manual_guard);
}

void UAbilityGuard::ActivateAbility(
    FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    UAbilitySystemComponent *ownerASC = ActorInfo->AbilitySystemComponent.Get();
    ownerASC->RemoveActiveEffectsWithTags(UAbilityBase::IdleTags);
    ownerASC->ApplyGameplayEffectToSelf(
        (UEffectOutOfCombat *)UEffectOutOfCombat::StaticClass()->GetDefaultObject(),
        1.0f,
        ownerASC->MakeEffectContext());

    FGameplayEffectContextHandle effectContext;
    int deflectCount = ownerASC->GetGameplayEffectCount(UEffectDeflect::StaticClass(), nullptr);
    if (!ownerASC->GetActiveGameplayEffect(this->GuardHandle) && deflectCount < 2)
    {
        this->DeflectHandle = ownerASC->ApplyGameplayEffectToSelf(
            (UEffectDeflect *)UEffectDeflect::StaticClass()->GetDefaultObject(),
            1.0f,
            effectContext);
        return;
    }
    if(deflectCount > 0)
    {
        ownerASC->RemoveActiveGameplayEffect(this->DeflectHandle);
    }
    this->GuardHandle = ownerASC->ApplyGameplayEffectToSelf(
        (UEffectGuard *)UEffectGuard::StaticClass()->GetDefaultObject(),
        1.0f,
        effectContext);
}