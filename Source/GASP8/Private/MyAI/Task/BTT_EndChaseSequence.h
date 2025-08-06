// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BTT_EndChaseSequence.generated.h"

/**
 * 
 */
UCLASS()
class UBTT_EndChaseSequence : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_EndChaseSequence();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName IsPatroling;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName IsChasing;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory);
};
