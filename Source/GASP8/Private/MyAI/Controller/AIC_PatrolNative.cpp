// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAI/Controller/AIC_PatrolNative.h"

AAIC_PatrolNative::AAIC_PatrolNative()
{
    this->PatrolTree = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/AI/BehaviorTree/BT_Patrol.BT_Patrol"));
    this->AIPerception = this->CreateDefaultSubobject<UAIPerceptionComponent>(FName("EnemyPerception"));

    UAISenseConfig_Sight *sightConfig = this->CreateDefaultSubobject<UAISenseConfig_Sight>(FName("SightConfig"));
    sightConfig->DetectionByAffiliation = FAISenseAffiliationFilter(false, true, false);
    this->AIPerception->ConfigureSense(*sightConfig);

    FScriptDelegate onTargetUpdatedDelegate;
    onTargetUpdatedDelegate.BindUFunction(this, FName("OnTargetUpdated"));
    this->AIPerception->OnTargetPerceptionUpdated.Add(onTargetUpdatedDelegate);
}

void AAIC_PatrolNative::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);
    this->RunBehaviorTree(this->PatrolTree);
}

void AAIC_PatrolNative::OnTargetUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if(!Actor->ActorHasTag(FName("Player")))
    {
        return;
    }
}