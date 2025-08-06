// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory);
};
