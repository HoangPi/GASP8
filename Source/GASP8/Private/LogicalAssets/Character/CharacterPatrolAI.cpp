// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicalAssets/Character/CharacterPatrolAI.h"

// Sets default values
ACharacterPatrolAI::ACharacterPatrolAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACharacterPatrolAI::BeginPlay()
{
	Super::BeginPlay();
	
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

