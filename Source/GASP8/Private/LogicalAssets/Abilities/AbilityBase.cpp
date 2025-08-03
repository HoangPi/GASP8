// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicalAssets/Abilities/AbilityBase.h"

#include "MyTags/MyTags.h"
#include "MyEffects/PlayerState/EffectOutOfCombat.h"

FGameplayTagContainer UAbilityBase::IdleTags;
FGameplayTagContainer UAbilityBase::CancelOnDisableTags;

UAbilityBase::UAbilityBase()
{
    UAbilityBase::IdleTags.AddTag(Tags::EffectType::idle);
    UAbilityBase::CancelOnDisableTags.AddTag(Tags::PlayerState::disabled);
    
    this->ActivationBlockedTags.AddTag(Tags::PlayerState::disabled);
    ((FGameplayTagContainer &)this->GetAssetTags()).AddTag(Tags::Ability::cancel_on_disable);
}

void UAbilityBase::ActivateAbility(
    FGameplayAbilitySpecHandle Handle, 
    const FGameplayAbilityActorInfo *ActorInfo, 
    FGameplayAbilityActivationInfo ActivationInfo, 
    const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    if(this->ShouldEnterCombat)
    {
        UAbilitySystemComponent *ownerASC = ActorInfo->AbilitySystemComponent.Get();
        ownerASC->RemoveActiveEffectsWithSourceTags(UAbilityBase::IdleTags);
        ownerASC->RemoveActiveGameplayEffectBySourceEffect(UEffectOutOfCombat::StaticClass(), nullptr);
    }
}

void UAbilityBase::EndAbility(
    FGameplayAbilitySpecHandle Handle, 
    const FGameplayAbilityActorInfo *ActorInfo, 
    FGameplayAbilityActivationInfo ActivationInfo, 
    bool bReplicateEndAbility, 
    bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    if(this->ShouldEnterCombat)
    {
        UAbilitySystemComponent *ownerASC = ActorInfo->AbilitySystemComponent.Get();
        ownerASC->ApplyGameplayEffectToSelf(
            (UGameplayEffect *)UEffectOutOfCombat::StaticClass()->GetDefaultObject(),
            1.0f,
            ownerASC->MakeEffectContext()
        );
    };
}