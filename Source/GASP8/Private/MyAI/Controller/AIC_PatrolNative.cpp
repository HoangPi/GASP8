// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAI/Controller/AIC_PatrolNative.h"

AAIC_PatrolNative::AAIC_PatrolNative()
{
    this->EnemyTree = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/AI/BehaviorTree/BT_Enemy.BT_Enemy"));
    this->AIPerception = this->CreateDefaultSubobject<UAIPerceptionComponent>(FName("EnemyPerception"));

    UAISenseConfig_Sight *sightConfig = this->CreateDefaultSubobject<UAISenseConfig_Sight>(FName("SightConfig"));
    sightConfig->DetectionByAffiliation = FAISenseAffiliationFilter(true, false, false);
    this->AIPerception->ConfigureSense(*sightConfig);

    this->Team = ETeamEnum::Enemy;

    FScriptDelegate onTargetUpdatedDelegate;
    onTargetUpdatedDelegate.BindUFunction(this, FName("OnTargetUpdated"));
    this->AIPerception->OnTargetPerceptionUpdated.Add(onTargetUpdatedDelegate);

    this->ChaseTargetKey = FName("ChaseTarget");
    this->LastLocationKey = FName("LastKnownLocation");
    this->IsPatrolingKey = FName("IsPatroling");
    this->IsChasingKey = FName("IsChasing");
}

void AAIC_PatrolNative::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);
    this->SetGenericTeamId(FGenericTeamId((uint8)this->Team));
    this->RunBehaviorTree(this->EnemyTree);
    this->Blackboard.Get()->SetValueAsBool(this->IsPatrolingKey, true);
}

void AAIC_PatrolNative::OnTargetUpdated(AActor *Actor, FAIStimulus Stimulus)
{
    if (Stimulus.WasSuccessfullySensed())
    {
        if (ACharacter *owner = Cast<ACharacter>(this->GetPawn()))
        {
            owner->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
        }
        this->Blackboard.Get()->SetValueAsObject(this->ChaseTargetKey, Actor);
        this->Blackboard.Get()->SetValueAsBool(this->IsPatrolingKey, false);
        this->Blackboard.Get()->SetValueAsBool(this->IsChasingKey, true);
        this->RunBehaviorTree(this->EnemyTree);
    }
    else
    {
        this->Blackboard.Get()->SetValueAsVector(this->LastLocationKey, Actor->GetActorLocation());
        this->Blackboard.Get()->SetValueAsObject(this->ChaseTargetKey, nullptr);
    }
}