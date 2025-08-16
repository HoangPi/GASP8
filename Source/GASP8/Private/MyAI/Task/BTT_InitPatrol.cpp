// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAI/Task/BTT_InitPatrol.h"

#include "LogicalAssets/Character/CharacterPatrolAI.h"
#include "Ultilities/Macro.h"
#include "Ultilities/GobalVars.h"

UAnimMontage *UBTT_InitPatrol::GenericPatrolAnimation;

UBTT_InitPatrol::UBTT_InitPatrol()
{
    this->PatrolTo = AI::Keys::PatrolTo;
    this->RotateTo = AI::Keys::RotateTo;
    this->WaitFor = AI::Keys::WaitFor;
    this->HasVisited = AI::Keys::HasVisited;
    this->AnimIdle = AI::Keys::AnimIdle;
    this->Finished = AI::Keys::Finished;
    // TODO: Promote this to global var
	this->PatrolSpeed = 200.0f;
    UBTT_InitPatrol::GenericPatrolAnimation = LoadObject<UAnimMontage>(nullptr, AssetPath::AnimMontage::DefaultPatrolAnim);
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
    owner->GetCharacterMovement()->MaxWalkSpeed = this->PatrolSpeed;
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(this->PatrolTo, owner->PatrolLocations[owner->CurrentPatrol]);
    OwnerComp.GetBlackboardComponent()->SetValueAsRotator(this->RotateTo, owner->PatrolRotations[owner->CurrentPatrol]);
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(this->WaitFor, owner->WaitTimes[owner->CurrentPatrol]);
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(this->HasVisited, true);
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(this->Finished, false);
    if(owner->PatrolAnimations.Num() > 0 && owner->CurrentPatrol < owner->PatrolAnimations.Num())
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(this->AnimIdle, owner->PatrolAnimations[owner->CurrentPatrol]);
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(this->AnimIdle, UBTT_InitPatrol::GenericPatrolAnimation);
    }
    return EBTNodeResult::Succeeded;
}