// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffects/Cooldown/EffectGenericCooldown.h"

#include "MyTags/MyTags.h"

UEffectGenericCooldown::UEffectGenericCooldown()
{
    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(5.0f);
}

void UEffectGenericCooldown::SetCooldown(float NewCooldown)
{
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(NewCooldown);
}