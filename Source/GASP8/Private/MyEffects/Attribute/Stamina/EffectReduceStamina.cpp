// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffects/Attribute/Stamina/EffectReduceStamina.h"

#include "MyAttributes/Stamina/AttributeStamina.h"
#include "MyTags/MyTags.h"

UEffectReduceStamina::UEffectReduceStamina()
{
    this->DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayModifierInfo mod;
    FSetByCallerFloat setByCaller;

    setByCaller.DataTag = Tags::Attribute::stamina;
    mod.Attribute = UAttributeStamina::GetStaminaAttribute();
    mod.ModifierOp = EGameplayModOp::Additive;
    mod.ModifierMagnitude = FGameplayEffectModifierMagnitude();

    this->Modifiers.Add(mod);
}