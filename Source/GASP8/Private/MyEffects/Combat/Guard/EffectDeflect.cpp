// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffects/Combat/Guard/EffectDeflect.h"

#include "Ultilities/GobalVars.h"

UEffectDeflect::UEffectDeflect()
{
    this->StackingType = EGameplayEffectStackingType::AggregateByTarget;
    this->StackLimitCount = 2;
    this->StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;
    this->StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::ClearEntireStack;

    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(DeflectDuration);
}