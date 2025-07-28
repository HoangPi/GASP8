// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffects/Attribute/Stamina/EffectRestoreStamina.h"

#include "MyAttributes/Stamina/AttributeStamina.h"
#include "MyTags/MyTags.h"

UEffectRestoreStamina::UEffectRestoreStamina()
{
    this->DurationPolicy = EGameplayEffectDurationType::Infinite;
    this->Period = 0.012f;
    FGameplayModifierInfo mod;
    FSetByCallerFloat callerMag;
    callerMag.DataTag = Tags::Attribute::stamina;
    mod.Attribute = UAttributeStamina::GetStaminaAttribute();
    mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(callerMag);
    mod.ModifierOp = EGameplayModOp::Additive;
    this->Modifiers.Add(mod);
    this->CachedAssetTags.AddTag(Tags::EffectType::idle);
}