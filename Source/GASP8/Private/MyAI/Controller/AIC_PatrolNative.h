// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

#include "AIC_PatrolNative.generated.h"

/**
 * 
 */
UCLASS()
class AAIC_PatrolNative : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_PatrolNative();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BehaviorTree")
	UBehaviorTree *PatrolTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BehaviorTree")
	UBehaviorTree *ChaseTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIPerception")
	UAIPerceptionComponent *AIPerception;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BehaviorTree")
	FName ChaseTargetKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BehaviorTree")
	FName LastLocationKey;

	virtual void OnPossess(APawn *InPawn);

	UFUNCTION()
	void OnTargetUpdated(AActor* Actor, FAIStimulus Stimulus);
};
