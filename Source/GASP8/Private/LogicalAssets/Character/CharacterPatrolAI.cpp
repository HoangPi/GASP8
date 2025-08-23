// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicalAssets/Character/CharacterPatrolAI.h"

#include "MyAI/Controller/AIC_PatrolNative.h"
#include "MyTags/MyTags.h"
#include "MyAttributes/Health/AttributeHealth.h"

// Sets default values
ACharacterPatrolAI::ACharacterPatrolAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->CurrentPatrol = 0;
	this->RetracePatrol = true,
	this->Direction = EPatrolDirection::FORAWRD;
	this->Team = ETeamEnum::Enemy;
	this->AbilitySystemComponent = this->CreateDefaultSubobject<UAbilitySystemComponent>(FName("EnemyASC"));
	this->AbilitySystemComponent->AddAttributeSetSubobject<UAttributeHealth>(
		this->CreateDefaultSubobject<UAttributeHealth>(FName("EmenyHealthPoint"))
	);
	// this->GiftAbility(UAbilityGetHit::StaticClass());
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

void ACharacterPatrolAI::NotifyHit(
	UPrimitiveComponent *MyComp, 
	AActor *Other, UPrimitiveComponent *OtherComp, 
	bool bSelfMoved, 
	FVector HitLocation, 
	FVector HitNormal, 
	FVector NormalImpulse, 
	const FHitResult &Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	// FGameplayEventData event;
	// event.EventTag = Tags::Ability::get_hit;
	// event.EventMagnitude = 50.0f;
	// this->AbilitySystemComponent->HandleGameplayEvent(Tags::Ability::get_hit, &event);
}