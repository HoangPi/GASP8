// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeStamina.generated.h"

/**
 * 
 */
UCLASS()
class UAttributeStamina : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeStamina();

	FGameplayAttributeData Stamina;
	FGameplayAttributeData MaxStamina;

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttributeStamina, Stamina);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttributeStamina, MaxStamina);
};
