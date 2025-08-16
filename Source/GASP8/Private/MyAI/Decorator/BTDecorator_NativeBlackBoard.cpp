// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAI/Decorator/BTDecorator_NativeBlackBoard.h"

UBTDecorator_NativeBlackBoard::UBTDecorator_NativeBlackBoard()
{

}

void UBTDecorator_NativeBlackBoard::SetNotifyObserver(TEnumAsByte<EBTBlackboardRestart::Type> observer)
{
    this->NotifyObserver = observer;
}

void UBTDecorator_NativeBlackBoard::SetBlackboardKey(FBlackboardKeySelector blackboardKey)
{
    this->BlackboardKey = blackboardKey;
}

void UBTDecorator_NativeBlackBoard::SetOperationType(uint8 operation)
{
    this->OperationType = operation;
}