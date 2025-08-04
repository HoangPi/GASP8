// Fill out your copyright notice in the Description page of Project Settings.

#include "MyComponents/Combat/Guard/ComponentGuard.h"

#include "GASP8/GASP8Character.h"
#include "MyAbilities/Combat/Guard/AbilityGuard.h"
#include "MyAbilities/Combat/Guard/AbilityGetHit.h"
#include "MyAbilities/Combat/DisableAndRecover/AbilityDisableAndRecover.h"

// Sets default values for this component's properties
UComponentGuard::UComponentGuard()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	if (IAbilitySystemInterface *owner = Cast<IAbilitySystemInterface>(this->GetOwner()))
	{
		this->OwnerASC = owner->GetAbilitySystemComponent();
		this->GuardHandle = this->OwnerASC->K2_GiveAbility(UAbilityGuard::StaticClass());
		this->OwnerASC->K2_GiveAbility(UAbilityGetHit::StaticClass());
		this->OwnerASC->K2_GiveAbility(UAbilityDisableAndRecover::StaticClass());
	}
	this->GuardAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Guard.IA_Guard"));
}

// Called when the game starts
void UComponentGuard::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (ACharacter *owner = Cast<ACharacter>(this->GetOwner()))
	{
		if (UEnhancedInputComponent *input = Cast<UEnhancedInputComponent>(owner->InputComponent))
		{
			this->SetupMyInputs(input);
		}
	}
}

void UComponentGuard::Guard()
{
	this->OwnerASC->TryActivateAbility(this->GuardHandle);
}

void UComponentGuard::StopGuarding()
{
	this->OwnerASC->CancelAbilityHandle(this->GuardHandle);
}

void UComponentGuard::SetupMyInputs(UEnhancedInputComponent *input)
{
	input->BindAction(this->GuardAction, ETriggerEvent::Started, this, &UComponentGuard::Guard);
	input->BindAction(this->GuardAction, ETriggerEvent::Completed, this, &UComponentGuard::StopGuarding);
}
