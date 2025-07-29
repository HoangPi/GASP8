// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffects/Cooldown/EffectGenericCooldown.h"

#include "MyTags/MyTags.h"

UEffectGenericCooldown::UEffectGenericCooldown()
{
    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    FSetByCallerFloat caller;
    caller.DataTag = Tags::EffectType::cooldown;
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(caller);
}