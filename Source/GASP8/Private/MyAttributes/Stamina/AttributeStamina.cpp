// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributes/Stamina/AttributeStamina.h"

#include "Ultilities/Macro.h"
#include "MyEffects/Attribute/Stamina/EffectRestoreStamina.h"
#include "MyTags/MyTags.h"

FGameplayTagContainer UAttributeStamina::AbilityTagToCancel;

UAttributeStamina::UAttributeStamina()
{
    UDataTable *table = LoadObject<UDataTable>(nullptr, TEXT("/Game/ThirdPerson/DataTable/DT_Stamina.DT_Stamina"));
    FAttributeMetaData *data = table->FindRow<FAttributeMetaData>(FName("2"), FString("Why"));
    this->MaxStamina = data->BaseValue;
    this->Stamina = data->BaseValue;
    if(!UAttributeStamina::AbilityTagToCancel.HasTagExact(Tags::Attribute::stamina))
    {
        UAttributeStamina::AbilityTagToCancel.AddTag(Tags::Attribute::stamina);
    }
}

void UAttributeStamina::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue)
{
    Super::PreAttributeBaseChange(Attribute, NewValue);
    float max;
    if(Attribute == UAttributeStamina::GetStaminaAttribute())
    {
        max = this->MaxStamina.GetCurrentValue();
        NewValue = CLAMP(NewValue, 0.0f, max);
        this->OnStaminaChange.Broadcast(NewValue/max);
    }
}

void UAttributeStamina::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
    Super::PostGameplayEffectExecute(Data);
    float current;
    if(Data.EvaluatedData.Attribute == UAttributeStamina::GetStaminaAttribute())
    {
        current = this->Stamina.GetCurrentValue();
        if(current >= this->MaxStamina.GetCurrentValue())
        {
            Data.Target.RemoveActiveGameplayEffectBySourceEffect(UEffectRestoreStamina::StaticClass(), nullptr);
        }
        else if(current <= 0.0f)
        {
            Data.Target.CancelAbilities(&UAttributeStamina::AbilityTagToCancel);
        }
    }
}