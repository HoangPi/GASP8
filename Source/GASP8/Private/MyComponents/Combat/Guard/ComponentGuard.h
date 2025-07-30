// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"

#include "ComponentGuard.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UComponentGuard : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponentGuard();

	class UAbilitySystemComponent *OwnerASC;
	class UInputAction *GuardAction;
	FGameplayAbilitySpecHandle GuardHandle;
	UFUNCTION()
	void Guard();
	UFUNCTION()
	void StopGuarding();
	inline void SetupMyInputs(UEnhancedInputComponent *input);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
