// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributes/Health/AttributeHealth.h"

#include "Ultilities/Macro.h"
#include "Ultilities/GobalVars.h"

UAttributeHealth::UAttributeHealth()
{
    UDataTable *table = LoadObject<UDataTable>(nullptr, AssetPath::DataTable::Health);
    FAttributeMetaData *data = table->FindRow<FAttributeMetaData>(FName("3"), FString("NoThanks"));
    this->Health = data->BaseValue;
    this->MaxHealth = data->BaseValue;
}

void UAttributeHealth::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);
    float max;
    if(Attribute == UAttributeHealth::GetHealthAttribute())
    {
        max = this->MaxHealth.GetCurrentValue();
        NewValue = CLAMP(NewValue, 0.0f, max);
        this->OnHealthChange.Broadcast(NewValue/max);
    }
}