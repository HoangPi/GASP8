// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAI/Task/BTT_InitPatrol.h"

#include "LogicalAssets/Character/CharacterPatrolAI.h"
#include "Ultilities/Macro.h"

UBTT_InitPatrol::UBTT_InitPatrol()
{
}

EBTNodeResult::Type UBTT_InitPatrol::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    ACharacterPatrolAI *owner = Cast<ACharacterPatrolAI>(OwnerComp.GetAIOwner()->GetPawn());
    if (!owner)
    {
        return EBTNodeResult::Aborted;
    }
    int max = owner->PatrolLocations.Num();
    owner->CurrentPatrol = CLAMP(owner->CurrentPatrol, 0, max);
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(this->PatrolTo, owner->PatrolLocations[owner->CurrentPatrol]);
    OwnerComp.GetBlackboardComponent()->SetValueAsRotator(this->RotateTo, owner->PatrolRotations[owner->CurrentPatrol]);
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(this->WaitFor, owner->WaitTimes[owner->CurrentPatrol]);
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(this->HasVisited, true);
    return EBTNodeResult::Succeeded;
}