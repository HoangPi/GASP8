// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributes/Stamina/AttributeStamina.h"

UAttributeStamina::UAttributeStamina()
{
    UDataTable *table = LoadObject<UDataTable>(nullptr, TEXT("/Game/ThirdPerson/DataTable/DT_Stamina.DT_Stamina"));
    FAttributeMetaData *data = table->FindRow<FAttributeMetaData>(FName("2"), FString("Why"));
    this->MaxStamina = data->BaseValue;
    this->Stamina = data->BaseValue;
}