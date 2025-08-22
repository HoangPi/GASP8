// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Animation/AnimMontage.h"
#include "Ultilities/TeamEnum.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "CharacterPatrolAI.generated.h"

UCLASS()
class ACharacterPatrolAI : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	enum EPatrolDirection : int32
	{
		STATIONARY = 0,
		FORAWRD = 1,
		BACKWARD = -1
	};

	ACharacterPatrolAI();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Patrol")
	TArray<FVector> PatrolLocations;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Patrol")
	TArray<FRotator> PatrolRotations;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Patrol")
	TArray<UAnimMontage *> PatrolAnimations;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Patrol")
	TArray<float> WaitTimes;

	UPROPERTY(BlueprintReadWrite, Category = "Patrol")
	int CurrentPatrol;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Patrol")
	bool RetracePatrol;

	UPROPERTY(BlueprintReadWrite, Category = "Patrol")
	int Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AIPerception")
	ETeamEnum Team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystemComponent")
	UAbilitySystemComponent *AbilitySystemComponent;

	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override { return this->AbilitySystemComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// public:
	// 	// Called every frame
	// 	virtual void Tick(float DeltaTime) override;

	// 	// Called to bind functionality to input
	// 	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
