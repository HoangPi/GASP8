// Fill out your copyright notice in the Description page of Project Settings.


#include "MyComponents/Movement/ComponentWallHug.h"

// Sets default values for this component's properties
UComponentWallHug::UComponentWallHug()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	this->InteractAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Interact.IA_Interact"));
}


// Called when the game starts
void UComponentWallHug::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(UEnhancedInputComponent *input = Cast<UEnhancedInputComponent>(this->GetOwner()->InputComponent))
	{
		input->BindAction(this->InteractAction, ETriggerEvent::Started, this, &UComponentWallHug::WallHug);
	}
}


// Called every frame
void UComponentWallHug::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UComponentWallHug::WallHug()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Hug"));
}