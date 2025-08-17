// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAI/BehaviorTree/BT_Enemy.h"

#include "MyAI/Task/BTT_InitPatrol.h"
#include "MyAI/Decorator/BTDecorator_NativeBlackBoard.h"
#include "MyAI/Blackboard/BB_Patrol.h"
#include "Ultilities/GobalVars.h"

UBT_Enemy::UBT_Enemy()
{
    this->RootNode = this->CreateDefaultSubobject<UBTComposite_Selector>(FName("RootPatrol"));
    this->BlackboardAsset = this->CreateDefaultSubobject<UBB_Patrol>(FName("PatrolTree"));

    FBTCompositeChild InitPatrolSequence;
    InitPatrolSequence.ChildComposite = this->CreateDefaultSubobject<UBTComposite_Sequence>(FName("InitPatrolSequence"));
    UBTT_InitPatrol *patrolTask = this->CreateDefaultSubobject<UBTT_InitPatrol>(FName("InitPatrol"));
    patrolTask->PatrolSpeed = 1000.0f;
    InitPatrolSequence.ChildTask = patrolTask;
    UBTDecorator_NativeBlackBoard *checkHasInit = this->CreateDefaultSubobject<UBTDecorator_NativeBlackBoard>(FName("HasInit"));
    checkHasInit->SetNotifyObserver(EBTBlackboardRestart::ResultChange);
    checkHasInit->SetOperationType(EBasicKeyOperation::NotSet);
    FBlackboardKeySelector hasInitKey;
    hasInitKey.SelectedKeyName = AI::Keys::HasInit;
    hasInitKey.SelectedKeyType = UBlackboardKeyType_Bool::StaticClass();
    checkHasInit->SetBlackboardKey(hasInitKey);
    this->RootNode.Get()->Children.Add(InitPatrolSequence);
}