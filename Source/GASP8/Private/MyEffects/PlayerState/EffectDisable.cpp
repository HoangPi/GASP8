// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffects/PlayerState/EffectDisable.h"

#include "MyTags/MyTags.h"

UEffectDisable::UEffectDisable()
{
    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(2.0f);

    UTargetTagsGameplayEffectComponent *comp = this->CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(FName("AddDisableTag"));
    ((FInheritedTagContainer &)comp->GetConfiguredTargetTagChanges()).AddTag(Tags::PlayerState::disabled);

    this->GEComponents.Add(comp);
}