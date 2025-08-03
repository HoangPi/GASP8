// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "GameplayEffect.h"

#include "ComponentLockon.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UComponentLockon : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComponentLockon();

	class UAbilitySystemComponent *OwnerASC;
	class AGASP8Character *MyOwner;
	UInputAction *LockonAction;
	UInputAction *LookAction;
	AActor *LockonTarget;
	FCollisionObjectQueryParams CollisionObject;
	FCollisionShape CollisionShape;
	FCollisionQueryParams ActorsToIgnore;
	FGameplayEffectSpec CooldownSpec;
	FActiveGameplayEffectHandle CooldownHandle;
	class UEffectGenericCooldown *cooldownEffect;

	inline void SetupMyInputs(UEnhancedInputComponent *input);
	UFUNCTION()
	void Lockon();
	UFUNCTION()
	void Switch(const FInputActionValue &Value);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
