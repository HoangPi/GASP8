// Fill out your copyright notice in the Description page of Project Settings.

#include "MyComponents/Movement/ComponentWallHug.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GASP8/GASP8Character.h"

// Sets default values for this component's properties
UComponentWallHug::UComponentWallHug()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	this->InteractAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Interact.IA_Interact"));
	this->MyOwner = Cast<AGASP8Character>(this->GetOwner());
}

// Called when the game starts
void UComponentWallHug::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (UEnhancedInputComponent *input = Cast<UEnhancedInputComponent>(this->GetOwner()->InputComponent))
	{
		input->BindAction(this->InteractAction, ETriggerEvent::Started, this, &UComponentWallHug::WallHug);
	}
}

// Called every frame
void UComponentWallHug::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UComponentWallHug::WallHug()
{
	if (this->MyOwner->IsHuggingWall)
	{
		this->MyOwner->IsHuggingWall = false;
		this->MyOwner->GetCharacterMovement()->bOrientRotationToMovement = true;
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Hug"));
	UCameraComponent *cam = ((AGASP8Character *)this->GetOwner())->GetFollowCamera();
	FVector start = cam->GetComponentLocation();
	FRotator rot = cam->GetComponentRotation();
	start += rot.Vector() * ((AGASP8Character *)this->GetOwner())->GetCameraBoom()->TargetArmLength;
	start.Z += 50;
	rot.Pitch = 0;
	FVector end = start + rot.Vector() * 100;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor *> ActorsToIgnore;
	FHitResult result;

	if (UKismetSystemLibrary::LineTraceSingleForObjects(
			this->GetWorld(),
			start,
			end,
			// 20,
			ObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			result,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.0f))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, result.GetActor()->GetName());
		// if(result.Distance <= 50)
		// {
		// 	return;
		// }
		// result.ImpactPoint.Z -= 50;
		result.ImpactPoint.Z = this->GetOwner()->GetActorLocation().Z;
		this->GetOwner()->SetActorLocationAndRotation(
			result.ImpactPoint,
			result.ImpactNormal.Rotation(),
			true);
		this->MyOwner->IsHuggingWall = true;
		this->MyOwner->GetCharacterMovement()->bOrientRotationToMovement = false;
		// this->GetOwner()->SetActorLocation(UKismetMathLibrary::VInterpTo(
		// 	this->GetOwner()->GetActorLocation(),
		// 	result.Location,
		// 	0.8f,
		// 	1
		// ));
	}
}

void UComponentWallHug::WallHugMovement(bool IsMovingLeft)
{
	this->MyOwner->AddMovementInput(this->MyOwner->GetActorRotation().RotateVector({0.0f, (IsMovingLeft ? 1.0f : -1.0f), 0.0f}), 1.0f);
}