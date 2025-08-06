// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "h")
	UAIPerceptionComponent *AIPerception;

	virtual void OnPossess(APawn *InPawn);

	UFUNCTION()
	void OnTargetUpdated(AActor* Actor, FAIStimulus Stimulus);
};
