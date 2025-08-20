// Fill out your copyright notice in the Description page of Project Settings.

#include "MyEffects/Cooldown/EffectGenericCooldown.h"

#include "MyTags/MyTags.h"
#include "Ultilities/GobalVars.h"

UEffectGenericCooldown::UEffectGenericCooldown()
{
    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    // this->DurationMagnitude = FGameplayEffectModifierMagnitude(EffectDuration::DefaultCooldown);
    FSetByCallerFloat setByCaller;
    setByCaller.DataTag = Tags::EffectType::cooldown;
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(setByCaller);
}