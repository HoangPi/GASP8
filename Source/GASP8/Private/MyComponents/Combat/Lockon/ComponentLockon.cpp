// Fill out your copyright notice in the Description page of Project Settings.


#include "MyComponents/Combat/Lockon/ComponentLockon.h"

// Sets default values for this component's properties
UComponentLockon::UComponentLockon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	this->SetComponentTickEnabled(false);
	this->LockonTarget = nullptr;
	this->LockonAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Lockon.IA_Lockon"));
	if(IAbilitySystemInterface *owner = this->GetOwner<IAbilitySystemInterface>())
	{
		this->OwnerASC = owner->GetAbilitySystemComponent();
	}

	// ...
}


// Called when the game starts
void UComponentLockon::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(ACharacter *owner = this->GetOwner<ACharacter>())
	{
		if(UEnhancedInputComponent *input = Cast<UEnhancedInputComponent>(owner->InputComponent))
		{
			this->SetupMyInputs(input);
		}
	}
	
}


// Called every frame
void UComponentLockon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UComponentLockon::SetupMyInputs(UEnhancedInputComponent *input)
{
	input->BindAction(this->LockonAction, ETriggerEvent::Started, this, &UComponentLockon::Lockon);
}

void UComponentLockon::Lockon()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString("Hello night city"));
}