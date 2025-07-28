// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"

#include "AttributeStamina.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyStaminaChange, float, Percentage);

UCLASS()
class UAttributeStamina : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeStamina();

	UPROPERTY()
	FGameplayAttributeData Stamina;
	UPROPERTY()
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintAssignable)
	FNotifyStaminaChange OnStaminaChange;

	inline virtual void PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue);
	inline virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttributeStamina, Stamina);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttributeStamina, MaxStamina);

	static FGameplayTagContainer AbilityTagToCancel;
};
