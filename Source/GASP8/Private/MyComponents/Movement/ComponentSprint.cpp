// Fill out your copyright notice in the Description page of Project Settings.


#include "MyComponents/Movement/ComponentSprint.h"

#include "GASP8/GASP8Character.h"
#include "MyAbilities/Movement/AbilitySprint.h"
#include "MyAttributes/Stamina/AttributeStamina.h"

// Sets default values for this component's properties
UComponentSprint::UComponentSprint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	if(IAbilitySystemInterface *owner = this->GetOwner<IAbilitySystemInterface>())
	{
		this->OwnerASC = owner->GetAbilitySystemComponent();
		OwnerASC->AddAttributeSetSubobject<UAttributeStamina>(this->CreateDefaultSubobject<UAttributeStamina>(FName("StaminaAttribute")));
		this->SprintHandle = this->OwnerASC->K2_GiveAbility(UAbilitySprint::StaticClass());
	}
	this->SprintAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Sprint.IA_Sprint"));
}


// Called when the game starts
void UComponentSprint::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(ACharacter *owner = GetOwner<ACharacter>())
	{
		if(UEnhancedInputComponent *input = Cast<UEnhancedInputComponent>(owner->InputComponent))
		{
			this->SetupMyInputs(input);
		}
	}
}

void UComponentSprint::Sprint()
{
	this->OwnerASC->TryActivateAbility(this->SprintHandle);
}

void UComponentSprint::StopSprinting()
{
	this->OwnerASC->CancelAbilityHandle(this->SprintHandle);
}

void UComponentSprint::SetupMyInputs(UEnhancedInputComponent *input)
{
	input->BindAction(this->SprintAction, ETriggerEvent::Started, this, &UComponentSprint::Sprint);
	input->BindAction(this->SprintAction, ETriggerEvent::Completed, this, &UComponentSprint::StopSprinting);
}