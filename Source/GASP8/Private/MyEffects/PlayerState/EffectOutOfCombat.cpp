// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffects/PlayerState/EffectOutOfCombat.h"

#include "MyEffects/Attribute/Stamina/EffectRestoreStamina.h"

UEffectOutOfCombat::UEffectOutOfCombat()
{
    this->StackingType = EGameplayEffectStackingType::AggregateByTarget;
    this->StackLimitCount = 1;
    this->StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;

    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(2.0f);

    UAdditionalEffectsGameplayEffectComponent *comp = this->CreateDefaultSubobject<UAdditionalEffectsGameplayEffectComponent>(FName("uhmmmm"));
    comp->OnCompleteNormal.Add(UEffectRestoreStamina::StaticClass());

    this->GEComponents.Add(comp);
}