// Fill out your copyright notice in the Description page of Project Settings.

#include "MyComponents/Combat/Lockon/ComponentLockon.h"

#include "GASP8/GASP8Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UComponentLockon::UComponentLockon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	this->SetComponentTickEnabled(false);
	this->LockonTarget = nullptr;
	this->LockonAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Lockon.IA_Lockon"));
	if (IAbilitySystemInterface *owner = this->GetOwner<IAbilitySystemInterface>())
	{
		this->OwnerASC = owner->GetAbilitySystemComponent();
	}
	this->CollisionObject.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	this->ActorsToIgnore.AddIgnoredActor(this->GetOwner());
	this->CollisionShape = FCollisionShape::MakeBox(FVector(100, 400, 400));
	// ...
}

// Called when the game starts
void UComponentLockon::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (AGASP8Character *owner = this->GetOwner<AGASP8Character>())
	{
		if (UEnhancedInputComponent *input = Cast<UEnhancedInputComponent>(owner->InputComponent))
		{
			this->SetupMyInputs(input);
		}
		this->MyOwner = owner;
	}
}

// Called every frame
void UComponentLockon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if(!this->LockonTarget)
	{
		this->SetComponentTickEnabled(false);
		return;
	}
	AController *controller = this->MyOwner->GetController();
	if(!controller->LineOfSightTo(this->LockonTarget, this->MyOwner->GetFollowCamera()->GetComponentLocation()))
	{
		this->LockonTarget = nullptr;
		this->SetComponentTickEnabled(false);
		return;
	}
	FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(this->MyOwner->GetActorLocation(), this->LockonTarget->GetActorLocation());
	targetRot.Pitch -= 20;
	controller->SetControlRotation(
		UKismetMathLibrary::RInterpTo(controller->GetControlRotation(), targetRot, DeltaTime, 10.0f)
	);
}

void UComponentLockon::SetupMyInputs(UEnhancedInputComponent *input)
{
	input->BindAction(this->LockonAction, ETriggerEvent::Started, this, &UComponentLockon::Lockon);
}

void UComponentLockon::Lockon()
{
	if(this->LockonTarget)
	{
		this->LockonTarget = nullptr;
		this->SetComponentTickEnabled(false);
		return;
	}
	FHitResult result;
	FVector start = this->MyOwner->GetFollowCamera()->GetComponentLocation();
	FRotator rot = this->MyOwner->GetFollowCamera()->GetComponentRotation();
	rot.Pitch += 15.0f;
	FVector rotVector = rot.Vector();
	start += rotVector * (this->MyOwner->GetCameraBoom()->TargetArmLength + this->CollisionShape.Box.HalfExtentX);
	FVector end = start + rotVector * 3000.0f;
	if (this->GetWorld()->SweepSingleByObjectType(
			result,
			start,
			end,
			FQuat(rot),
			this->CollisionObject,
			this->CollisionShape,
			this->ActorsToIgnore))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, result.GetActor()->GetName());
		this->LockonTarget = result.GetActor();
		this->SetComponentTickEnabled(true);
	}
	UKismetSystemLibrary::BoxTraceSingle(
		this->GetWorld(),
		start,
		end,
		FVector(100, 400, 400),
		rot,
		ETraceTypeQuery::TraceTypeQuery_MAX,
		false,
		TArray<AActor *>(),
		EDrawDebugTrace::ForDuration,
		result,
		false,
		FLinearColor::Blue);
}