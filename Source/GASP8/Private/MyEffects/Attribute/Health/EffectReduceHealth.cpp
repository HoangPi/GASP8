// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffects/Attribute/Health/EffectReduceHealth.h"

#include "MyAttributes/Health/AttributeHealth.h"
#include "MyTags/MyTags.h"

UEffectReduceHealth::UEffectReduceHealth()
{
    this->DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayModifierInfo mod;
    FSetByCallerFloat callerSetter;

    callerSetter.DataTag = Tags::Attribute::health;

    mod.Attribute = UAttributeHealth::GetHealthAttribute();
    mod.ModifierOp = EGameplayModOp::Additive;
    mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(callerSetter);
    this->Modifiers.Add(mod);
}