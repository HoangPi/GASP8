// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffects/Combat/Guard/EffectGuard.h"

#include "Ultilities/GobalVars.h"

UEffectGuard::UEffectGuard()
{
    this->StackingType = EGameplayEffectStackingType::AggregateByTarget;
    this->StackLimitCount = 1;
    this->StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;
    this->StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::ClearEntireStack;

    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(2.0f);
}