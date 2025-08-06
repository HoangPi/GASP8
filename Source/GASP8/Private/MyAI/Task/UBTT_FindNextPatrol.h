// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "UBTT_FindNextPatrol.generated.h"

/**
 * 
 */
UCLASS()
class UUBTT_FindNextPatrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UUBTT_FindNextPatrol();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName PatrolTo;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName RotateTo;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName WaitFor;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName Finished;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName AnimIdle;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName IsPatroling;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory);
};
