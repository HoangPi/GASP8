// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "EnhancedInputComponent.h"

#include "ComponentSprint.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UComponentSprint : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponentSprint();

	UAbilitySystemComponent *OwnerASC;
	UInputAction *SprintAction;
	FGameplayAbilitySpecHandle SprintHandle;

	inline void SetupMyInputs(UEnhancedInputComponent *input);

	void Sprint();
	void StopSprinting();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

// public:	
// 	// Called every frame
// 	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
