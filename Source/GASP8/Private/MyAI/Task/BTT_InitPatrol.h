// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BTT_InitPatrol.generated.h"

/**
 * 
 */
UCLASS()
class UBTT_InitPatrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_InitPatrol();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName PatrolTo;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName RotateTo;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName WaitFor;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName HasVisited;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName AnimIdle;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName Finished;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float PatrolSpeed;

	static UAnimMontage *GenericPatrolAnimation;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory);
};
