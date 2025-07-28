// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributes/Stamina/AttributeStamina.h"

#include "Ultilities/Macro.h"

UAttributeStamina::UAttributeStamina()
{
    UDataTable *table = LoadObject<UDataTable>(nullptr, TEXT("/Game/ThirdPerson/DataTable/DT_Stamina.DT_Stamina"));
    FAttributeMetaData *data = table->FindRow<FAttributeMetaData>(FName("2"), FString("Why"));
    this->MaxStamina = data->BaseValue;
    this->Stamina = data->BaseValue;
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
