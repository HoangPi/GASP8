// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffects/Cooldown/EffectGenericCooldown.h"

#include "MyTags/MyTags.h"
#include "Ultilities/GobalVars.h"

UEffectGenericCooldown::UEffectGenericCooldown()
{
    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(EffectDuration::DefaultCooldown);
}

void UEffectGenericCooldown::SetCooldown(float NewCooldown)
{
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(NewCooldown);
}