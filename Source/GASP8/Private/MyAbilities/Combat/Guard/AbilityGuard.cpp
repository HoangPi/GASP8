// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAbilities/Combat/Guard/AbilityGuard.h"

#include "MyEffects/PlayerState/EffectOutOfCombat.h"
#include "MyTags/MyTags.h"

UAbilityGuard::UAbilityGuard()
{
    this->ShouldEnterCombat = false;
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
        ownerASC->MakeEffectContext()
    );
}