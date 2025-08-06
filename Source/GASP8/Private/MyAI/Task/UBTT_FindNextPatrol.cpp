// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAI/Task/UBTT_FindNextPatrol.h"

#include "LogicalAssets/Character/CharacterPatrolAI.h"
#include "Ultilities/Macro.h"

UUBTT_FindNextPatrol::UUBTT_FindNextPatrol()
{

}

EBTNodeResult::Type UUBTT_FindNextPatrol::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    EBTNodeResult::Type earlyCheck = Super::ExecuteTask(OwnerComp, NodeMemory);
    if(earlyCheck != EBTNodeResult::Type::Succeeded)
    {
        return earlyCheck;
    }
    ACharacterPatrolAI *owner = Cast<ACharacterPatrolAI>(OwnerComp.GetAIOwner()->GetPawn());
    if (!owner)
    {
        return EBTNodeResult::Aborted;
    }
    owner->CurrentPatrol += owner->Direction;
    if(owner->CurrentPatrol >= owner->PatrolLocations.Num() || owner->CurrentPatrol < 0)
    {
        if(owner->RetracePatrol)
        {
            owner->Direction *= -1;
            owner->CurrentPatrol += owner->Direction * 2;
        }
        else
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsBool(this->Finished, true);
            return EBTNodeResult::Succeeded;
        }
    }
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(this->PatrolTo, owner->PatrolLocations[owner->CurrentPatrol]);
    OwnerComp.GetBlackboardComponent()->SetValueAsRotator(this->RotateTo, owner->PatrolRotations[owner->CurrentPatrol]);
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(this->WaitFor, owner->WaitTimes[owner->CurrentPatrol]);
    if(owner->PatrolAnimations.Num() > 0 && owner->CurrentPatrol < owner->PatrolAnimations.Num())
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(this->AnimIdle, owner->PatrolAnimations[owner->CurrentPatrol]);
    }
    return EBTNodeResult::Succeeded;
}