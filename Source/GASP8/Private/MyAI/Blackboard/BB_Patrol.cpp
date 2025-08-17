// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAI/Blackboard/BB_Patrol.h"

#include "Ultilities/GobalVars.h"

UBB_Patrol::UBB_Patrol()
{
    // Just overwrite this, more local vars == headache
    FBlackboardEntry tempKey;
    UBlackboardKeyType_Object *animObjKey = this->CreateDefaultSubobject<UBlackboardKeyType_Object>(FName("AnimKey"));
    animObjKey->BaseClass = UAnimMontage::StaticClass();

    tempKey.EntryName = AI::Keys::PatrolTo;
    tempKey.KeyType = UBlackboardKeyType_Vector::StaticClass()->GetDefaultObject<UBlackboardKeyType>();
    this->Keys.Add(tempKey);

    tempKey.EntryName = AI::Keys::RotateTo;
    this->Keys.Add(tempKey);

    tempKey.EntryName = AI::Keys::WaitFor;
    tempKey.KeyType = UBlackboardKeyType_Float::StaticClass()->GetDefaultObject<UBlackboardKeyType>();
    this->Keys.Add(tempKey);

    tempKey.EntryName = AI::Keys::HasVisited;
    tempKey.KeyType = UBlackboardKeyType_Bool::StaticClass()->GetDefaultObject<UBlackboardKeyType>();
    this->Keys.Add(tempKey);

    tempKey.EntryName = AI::Keys::AnimIdle;
    tempKey.KeyType = animObjKey;
    this->Keys.Add(tempKey);

    tempKey.EntryName = AI::Keys::Finished;
    tempKey.KeyType = UBlackboardKeyType_Bool::StaticClass()->GetDefaultObject<UBlackboardKeyType>();
    this->Keys.Add(tempKey);

    tempKey.EntryName = AI::Keys::IsPatrolling;
    tempKey.KeyType = UBlackboardKeyType_Bool::StaticClass()->GetDefaultObject<UBlackboardKeyType>();
    this->Keys.Add(tempKey);
}