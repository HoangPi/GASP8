// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAI/Task/BTT_EndChaseSequence.h"

#include "MyAI/Controller/AIC_PatrolNative.h"

UBTT_EndChaseSequence::UBTT_EndChaseSequence()
{
}

EBTNodeResult::Type UBTT_EndChaseSequence::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    EBTNodeResult::Type earlyCheck = Super::ExecuteTask(OwnerComp, NodeMemory);
    if(earlyCheck != EBTNodeResult::Succeeded)
    {
        return earlyCheck;
    }
    if(ACharacter *owner = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn()))
    {
        owner->GetCharacterMovement()->MaxWalkSpeed = 120.0f;
    }
    if(AAIC_PatrolNative *controller = Cast<AAIC_PatrolNative>(OwnerComp.GetAIOwner()))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(this->IsChasing, false);
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(this->IsPatroling, true);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}