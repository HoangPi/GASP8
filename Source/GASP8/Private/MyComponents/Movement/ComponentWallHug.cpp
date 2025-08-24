// Fill out your copyright notice in the Description page of Project Settings.

#include "MyComponents/Movement/ComponentWallHug.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GASP8/GASP8Character.h"
#include "Ultilities/Macro.h"

double UComponentWallHug::CameraPeekSpeed = 500.0f;
double UComponentWallHug::CameraMaxPeekDistance = 50.0f;

// Sets default values for this component's properties
UComponentWallHug::UComponentWallHug()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	this->InteractAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Interact.IA_Interact"));
	this->MyOwner = Cast<AGASP8Character>(this->GetOwner());
	this->IsHuggingWall = false;
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
	if (this->IsHuggingWall)
	{
		this->UpdateIsHuggingWall(false);
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
		result.ImpactPoint.Z = this->GetOwner()->GetActorLocation().Z;
		this->GetOwner()->SetActorLocationAndRotation(
			result.ImpactPoint,
			result.ImpactNormal.Rotation(),
			true);
		this->UpdateIsHuggingWall(true);
		this->MyOwner->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void UComponentWallHug::WallHugMovement(bool IsMovingLeft)
{
	FVector start = this->MyOwner->GetActorLocation();
	start += this->MyOwner->GetActorRotation().RotateVector({0.0f, (IsMovingLeft ? 1.0f : -1.0f), 0.0f}) * 50;
	FVector end = start + (-this->MyOwner->GetActorRotation().Vector()) * 100.0f;
	FHitResult result;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor *> ActorsToIgnore;
	if (UKismetSystemLibrary::LineTraceSingleForObjects(
			this->GetWorld(),
			start,
			end,
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
		this->MyOwner->AddMovementInput(this->MyOwner->GetActorRotation().RotateVector({0.0f, (IsMovingLeft ? 1.0f : -1.0f), 0.0f}), 1.0f);
	}
	else
	{
		auto origin = this->MyOwner->GetFollowCamera()->GetRelativeLocation();
		origin.Y = origin.Y + (IsMovingLeft ? -1.0f : 1.0f) * this->GetWorld()->GetDeltaSeconds() * UComponentWallHug::CameraPeekSpeed;
		origin.Y = CLAMP(origin.Y, -UComponentWallHug::CameraMaxPeekDistance, UComponentWallHug::CameraMaxPeekDistance);
		this->MyOwner->GetFollowCamera()->SetRelativeLocation(origin);
	}
}

void UComponentWallHug::ResetCamera()
{
	auto origin = this->MyOwner->GetFollowCamera()->GetRelativeLocation();
	if (UKismetMathLibrary::Abs(origin.Y) <= 1.0f)
	{
		this->MyOwner->GetFollowCamera()->SetRelativeLocation(FVector{0.0f});
		return;
	}
	double scaleDirection = origin.Y < 0 ? 1.0f : -1.0f;
	origin.Y += scaleDirection * this->GetWorld()->GetDeltaSeconds() * UComponentWallHug::CameraPeekSpeed;
	if ((scaleDirection == 1.0f && origin.Y >= 0) || (scaleDirection == -1.0f && origin.Y <= 0))
	{
		this->MyOwner->GetFollowCamera()->SetRelativeLocation(FVector{0.0f});
		return;
	}
	this->MyOwner->GetFollowCamera()->SetRelativeLocation(origin);
	this->GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
															{ this->ResetCamera(); });
}

void UComponentWallHug::UpdateIsHuggingWall(bool state)
{
	this->IsHuggingWall = state;
	if(!state)
	{
		this->ResetCamera();
	}
}