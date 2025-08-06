// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAI/Controller/AIC_PatrolNative.h"

AAIC_PatrolNative::AAIC_PatrolNative()
{
    this->EnemyTree = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/AI/BehaviorTree/BT_Enemy.BT_Enemy"));
    this->AIPerception = this->CreateDefaultSubobject<UAIPerceptionComponent>(FName("EnemyPerception"));

    UAISenseConfig_Sight *sightConfig = this->CreateDefaultSubobject<UAISenseConfig_Sight>(FName("SightConfig"));
    sightConfig->DetectionByAffiliation = FAISenseAffiliationFilter(true, true, false);
    this->AIPerception->ConfigureSense(*sightConfig);

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
    this->RunBehaviorTree(this->EnemyTree);
    this->Blackboard.Get()->SetValueAsBool(this->IsPatrolingKey, true);
}

void AAIC_PatrolNative::OnTargetUpdated(AActor *Actor, FAIStimulus Stimulus)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, Actor->GetName());
    if (!Actor->ActorHasTag(FName("Player")))
    {
        return;
    }
    if (Stimulus.WasSuccessfullySensed())
    {
        if (ACharacter *owner = Cast<ACharacter>(this->GetPawn()))
        {
            owner->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
        }
        this->Blackboard.Get()->SetValueAsObject(this->ChaseTargetKey, Actor);
        this->Blackboard.Get()->SetValueAsBool(this->IsPatrolingKey, false);
        this->Blackboard.Get()->SetValueAsBool(this->IsChasingKey, true);
    }
    else
    {
        this->Blackboard.Get()->SetValueAsVector(this->LastLocationKey, Actor->GetActorLocation());
        this->Blackboard.Get()->SetValueAsObject(this->ChaseTargetKey, nullptr);
    }
}