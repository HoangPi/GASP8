// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"

#include "BehaviorTree/Composites/BTComposite_Selector.h"
#include "BehaviorTree/Composites/BTComposite_Sequence.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

#include "BT_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class UBT_Enemy : public UBehaviorTree
{
	GENERATED_BODY()

public:
	UBT_Enemy();
};
