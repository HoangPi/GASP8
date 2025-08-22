// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicalAssets/Character/CharacterPatrolAI.h"

#include "MyAI/Controller/AIC_PatrolNative.h"

// Sets default values
ACharacterPatrolAI::ACharacterPatrolAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->CurrentPatrol = 0;
	this->RetracePatrol = true,
	this->Direction = EPatrolDirection::FORAWRD;
	this->Team = ETeamEnum::Enemy;
}

// Called when the game starts or when spawned
void ACharacterPatrolAI::BeginPlay()
{
	Super::BeginPlay();
	((AAIC_PatrolNative *)this->GetController())->SetTeam(this->Team);
}

// // Called every frame
// void ACharacterPatrolAI::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);

// }

// // Called to bind functionality to input
// void ACharacterPatrolAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);

// }

