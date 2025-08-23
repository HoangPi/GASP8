// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAbilities/Combat/Guard/AbilityGetHit.h"

#include "MyTags/MyTags.h"
#include "MyAbilities/Combat/Guard/AbilityGuard.h"
#include "MyEffects/Attribute/Health/EffectReduceHealth.h"
#include "MyEffects/Attribute/Stamina/EffectReduceStamina.h"
#include "MyAttributes/Stamina/AttributeStamina.h"
#include "MyEffects/PlayerState/EffectDisable.h"
#include "GASP8/GASP8Character.h"
#include "MyAbilities/Combat/DisableAndRecover/AbilityDisableAndRecover.h"

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
    this->ActivationBlockedTags.RemoveTag(Tags::PlayerState::disabled);
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
    FGameplayEffectContextHandle context = ownerASC->MakeEffectContext();
    // Could have init right here
    FGameplayEffectSpec staminaSpec;
    bool unused;

    // If is deflecting
    // But for skisue branch it can skip the init steps
    if (this->DeflectHandle && ownerASC->GetActiveGameplayEffect(*this->DeflectHandle))
    {
        ownerASC->RemoveActiveGameplayEffect(*this->DeflectHandle, 1);
        staminaSpec = FGameplayEffectSpec((UEffectReduceStamina *)UEffectReduceStamina::StaticClass()->GetDefaultObject(), context);
        staminaSpec.SetSetByCallerMagnitude(Tags::Attribute::stamina, -TriggerEventData->EventMagnitude / 2);
        ownerASC->ApplyGameplayEffectSpecToSelf(staminaSpec);
        // this->EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
    }
    // If is "guarding"
    else if (this->GuardHandle && ownerASC->GetActiveGameplayEffect(*this->GuardHandle))
    {
        ownerASC->RemoveActiveGameplayEffect(*this->GuardHandle, 1);
        staminaSpec = FGameplayEffectSpec((UEffectReduceStamina *)UEffectReduceStamina::StaticClass()->GetDefaultObject(), context);
        staminaSpec.SetSetByCallerMagnitude(Tags::Attribute::stamina, -TriggerEventData->EventMagnitude);
        ownerASC->ApplyGameplayEffectSpecToSelf(staminaSpec);
        if (ownerASC->GetGameplayAttributeValue(UAttributeStamina::GetStaminaAttribute(), unused) <= 0.0f)
        {
            ownerASC->TryActivateAbilityByClass(UAbilityDisableAndRecover::StaticClass());
        }
    }
    // If is guarding
    else if (ownerASC->HasMatchingGameplayTag(Tags::PlayerState::manual_guard))
    {
        staminaSpec = FGameplayEffectSpec((UEffectReduceStamina *)UEffectReduceStamina::StaticClass()->GetDefaultObject(), context);
        staminaSpec.SetSetByCallerMagnitude(Tags::Attribute::stamina, -TriggerEventData->EventMagnitude);
        ownerASC->ApplyGameplayEffectSpecToSelf(staminaSpec);
        if (ownerASC->GetGameplayAttributeValue(UAttributeStamina::GetStaminaAttribute(), unused) <= 0.0f)
        {
            ownerASC->TryActivateAbilityByClass(UAbilityDisableAndRecover::StaticClass());
        }
    }
    // Skisue
    else
    {
        FGameplayEffectSpec spec = FGameplayEffectSpec(
            (UEffectReduceHealth *)UEffectReduceHealth::StaticClass()->GetDefaultObject(),
            context);
        spec.SetSetByCallerMagnitude(Tags::Attribute::health, -TriggerEventData->EventMagnitude);
        ownerASC->ApplyGameplayEffectSpecToSelf(spec);
    }
    this->EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UAbilityGetHit::OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);
    if(!Cast<AGASP8Character>(ActorInfo->AvatarActor.Get()))
    {
        return;
    }
    UAbilityGuard *guard = (UAbilityGuard *)ActorInfo->AbilitySystemComponent.Get()->FindAbilitySpecFromClass(UAbilityGuard::StaticClass())->GetPrimaryInstance();
    this->DeflectHandle = &guard->DeflectHandle;
    this->GuardHandle = &guard->GuardHandle;
}