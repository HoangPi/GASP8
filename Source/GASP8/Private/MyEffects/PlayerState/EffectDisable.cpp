// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffects/PlayerState/EffectDisable.h"

#include "MyTags/MyTags.h"
#include "MyEffects/PlayerState/EffectOutOfCombat.h"

UEffectDisable::UEffectDisable()
{
    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(2.0f);

    UTargetTagsGameplayEffectComponent *comp = this->CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(FName("AddDisableTag"));
    ((FInheritedTagContainer &)comp->GetConfiguredTargetTagChanges()).AddTag(Tags::PlayerState::disabled);

    UAdditionalEffectsGameplayEffectComponent *comp2 = this->CreateDefaultSubobject<UAdditionalEffectsGameplayEffectComponent>(FName("CallAmbulance"));
    FConditionalGameplayEffect additionalEffect;
    additionalEffect.EffectClass = UEffectOutOfCombat::StaticClass();
    comp2->OnApplicationGameplayEffects.Add(additionalEffect);

    this->GEComponents.Add(comp);
    this->GEComponents.Add(comp2);
}