// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"

#include "BehaviorTree/Blackboard/BlackboardKeyEnums.h"

#include "BTDecorator_NativeBlackBoard.generated.h"

/**
 * 
 */
UCLASS()
class UBTDecorator_NativeBlackBoard : public UBTDecorator_Blackboard
{
	GENERATED_BODY()

public:
	UBTDecorator_NativeBlackBoard();

	void SetNotifyObserver(TEnumAsByte<EBTBlackboardRestart::Type> observer);
	void SetBlackboardKey(FBlackboardKeySelector BlackboardKey);

	// Set operation can either be Basic, Arithmetic or Text
	// Choose the enum from EBasicKeyOperation, EArithmeticKeyOperation, ETextKeyOperation
	void SetOperationType(uint8 operation);
};
