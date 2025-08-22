// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeHealth.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyHealthChange, float, Percentage);

UCLASS()
class UAttributeHealth : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAttributeHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintAssignable)
	FNotifyHealthChange OnHealthChange;

	inline virtual void PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue);
	// inline virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data);
	
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttributeHealth, Health);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttributeHealth, MaxHealth);
};
