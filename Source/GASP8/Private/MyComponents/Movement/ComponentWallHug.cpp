// Fill out your copyright notice in the Description page of Project Settings.

#include "MyComponents/Movement/ComponentWallHug.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GASP8/GASP8Character.h"
#include "Ultilities/Macro.h"

double UComponentWallHug::CameraPeekSpeed = 5.0f;
double UComponentWallHug::CameraMaxPeekDistance = 50.0f;
FCollisionObjectQueryParams UComponentWallHug::TraceObjects;
FCollisionQueryParams UComponentWallHug::ActorsToIgnores;

// Sets default values for this component's properties
UComponentWallHug::UComponentWallHug()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	this->InteractAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Interact.IA_Interact"));
	this->MyOwner = Cast<AGASP8Character>(this->GetOwner());
	this->IsHuggingWall = false;
	this->PeekState = PeekDirection::NONE;
	// UComponentWallHug::TraceObjects.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
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
	// Dont want to add non-player/CDO objects
	this->OriginCameraLength = this->MyOwner->GetCameraBoom()->TargetArmLength;
	UComponentWallHug::ActorsToIgnores.AddIgnoredActor(this->GetOwner());
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
		// this->MyOwner->GetCharacterMovement()->bOrientRotationToMovement = true;
		return;
	}
	UCameraComponent *cam = this->MyOwner->GetFollowCamera();
	FVector start = cam->GetComponentLocation();
	FRotator rot = cam->GetComponentRotation();
	start += rot.Vector() * this->MyOwner->GetCameraBoom()->TargetArmLength;
	start.Z += 50;
	rot.Pitch = 0;
	FVector end = start + rot.Vector() * 150;
	// TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	// ObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor *> ActorsToIgnore;
	FHitResult result;

	if (this->GetWorld()->LineTraceSingleByObjectType(
			result,
			start,
			end,
			UComponentWallHug::TraceObjects,
			UComponentWallHug::ActorsToIgnores))
	{
		// Only if adjacent area are flat and big enough
		FHitResult unusedResult;
		start = result.Location + result.Normal * 50;
		constexpr double width = 160.0f;
		end = result.Location;
		end -= result.ImpactNormal * 50;
		end += result.ImpactNormal.Rotation().RotateVector({0, 1, 0}) * width;
		UKismetSystemLibrary::LineTraceSingle(this->GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery_MAX, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, unusedResult, false, FLinearColor::Blue, FLinearColor::Blue);
		if (this->GetWorld()->LineTraceSingleByObjectType(unusedResult, start, end, UComponentWallHug::TraceObjects, UComponentWallHug::ActorsToIgnores))
		{
			goto setup_hugging_wall;
		}
		end += result.ImpactNormal.Rotation().RotateVector({0, -1, 0}) * 2 * width;
		UKismetSystemLibrary::LineTraceSingle(this->GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery_MAX, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, unusedResult, false, FLinearColor::Blue, FLinearColor::Blue);
		if (!this->GetWorld()->LineTraceSingleByObjectType(unusedResult, start, end, UComponentWallHug::TraceObjects, UComponentWallHug::ActorsToIgnores))
		{
			return;
		}

	setup_hugging_wall:
		this->ZoomIn(this->MyOwner->GetCameraBoom());
		result.ImpactPoint.Z = this->MyOwner->GetActorLocation().Z;
		this->MyOwner->SetActorLocationAndRotation(
			result.ImpactPoint,
			result.ImpactNormal.Rotation(),
			true);
		this->UpdateIsHuggingWall(true);
		// this->MyOwner->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void UComponentWallHug::WallHugMovement(bool IsMovingRight)
{
	if (this->PeekState != PeekDirection::NONE)
	{
		if ((IsMovingRight && this->PeekState == PeekDirection::RIGHT) || (!IsMovingRight && this->PeekState == PeekDirection::LEFT))
		{
			return;
		}
		this->UnPeek(this->MyOwner->GetFollowCamera(), this->PeekState);
		this->PeekState = NONE;
	}
	constexpr double AngluarCheck = 135.0f;
	FVector start = this->MyOwner->GetActorLocation();
	FRotator actorRotation = this->MyOwner->GetActorRotation();
	FRotator rotateCopy = actorRotation;
	rotateCopy.Yaw += (IsMovingRight ? AngluarCheck : -AngluarCheck);
	start += rotateCopy.Vector() * 25;
	FVector end = start + rotateCopy.Vector() * 100;
	// start += this->MyOwner->GetActorRotation().RotateVector({0.0f, (IsMovingRight ? 1.0f : -1.0f), 0.0f}) * 28;
	// FVector end = start + (-this->MyOwner->GetActorRotation().Vector()) * 75.0f;
	FHitResult result;
	// TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	// ObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	// TArray<AActor *> ActorsToIgnore;
	if (this->GetWorld()->LineTraceSingleByObjectType(
			result,
			start,
			end,
			UComponentWallHug::TraceObjects,
			UComponentWallHug::ActorsToIgnores))
	{
		this->MyOwner->AddMovementInput(actorRotation.RotateVector({0.0f, (IsMovingRight ? 1.0f : -1.0f), 0.0f}), 1.0f);
		this->MyOwner->AddMovementInput(actorRotation.RotateVector(FVector::BackwardVector), 0.5f);
		if (this->GetWorld()->LineTraceSingleByObjectType(
				result,
				start, start + actorRotation.RotateVector(FVector::BackwardVector) * 100,
				UComponentWallHug::TraceObjects,
				UComponentWallHug::ActorsToIgnores))
		{
			this->MyOwner->SetActorRotation(
				FRotator(0.0f, UKismetMathLibrary::FInterpTo(actorRotation.Yaw, result.ImpactNormal.Rotation().Yaw, this->GetWorld()->GetDeltaSeconds(), 5), 0.0f));
		}
	}
	else
	{
		PeekDirection temp = (IsMovingRight ? PeekDirection::RIGHT : PeekDirection::LEFT);
		this->PeekState = temp;
		this->Peek(this->MyOwner->GetFollowCamera(), temp);
		// auto origin = this->MyOwner->GetFollowCamera()->GetRelativeLocation();
		// origin.Y = origin.Y + (IsMovingRight ? -1.0f : 1.0f) * this->GetWorld()->GetDeltaSeconds() * UComponentWallHug::CameraPeekSpeed;
		// origin.Y = CLAMP(origin.Y, -UComponentWallHug::CameraMaxPeekDistance, UComponentWallHug::CameraMaxPeekDistance);
		// this->MyOwner->GetFollowCamera()->SetRelativeLocation(origin);
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

void UComponentWallHug::ZoomIn(USpringArmComponent *SpringArm, double time)
{
	time += this->GetWorld()->GetDeltaSeconds() * UComponentWallHug::CameraPeekSpeed;
	if (time >= 1)
	{
		SpringArm->TargetArmLength = this->OriginCameraLength - UComponentWallHug::CameraMaxOffSetY;
		return;
	}
	SpringArm->TargetArmLength = this->OriginCameraLength - UKismetMathLibrary::FInterpEaseInOut(0, UComponentWallHug::CameraMaxOffSetY, time, 2);
	this->GetWorld()->GetTimerManager().SetTimerForNextTick([this, SpringArm, time]()
															{ this->ZoomIn(SpringArm, time); });
}

void UComponentWallHug::ZoomOut(USpringArmComponent *SpringArm, double time)
{
	time += this->GetWorld()->GetDeltaSeconds() * UComponentWallHug::CameraPeekSpeed;
	if (time >= 1)
	{
		SpringArm->TargetArmLength = this->OriginCameraLength;
		return;
	}
	SpringArm->TargetArmLength = this->OriginCameraLength - UKismetMathLibrary::FInterpEaseInOut(UComponentWallHug::CameraMaxOffSetY, 0, time, 2);
	this->GetWorld()->GetTimerManager().SetTimerForNextTick([this, SpringArm, time]()
															{ this->ZoomOut(SpringArm, time); });
}

void UComponentWallHug::Peek(UCameraComponent *FollowCamera, PeekDirection Direction, double time)
{
	time += this->GetWorld()->GetDeltaSeconds() * UComponentWallHug::CameraPeekSpeed;
	double target = Direction == PeekDirection::LEFT ? UComponentWallHug::CameraMaxOffSetX : -UComponentWallHug::CameraMaxOffSetX;
	if (time >= 1)
	{
		FollowCamera->SetRelativeLocation(FVector{0.0f, target, 0.0f});
		return;
	}
	FollowCamera->SetRelativeLocation(FVector{
		0.0f,
		UKismetMathLibrary::FInterpEaseInOut(0, target, time, 2),
		0.0f});
	this->GetWorld()->GetTimerManager().SetTimerForNextTick([this, FollowCamera, Direction, time]()
															{ this->Peek(FollowCamera, Direction, time); });
}
void UComponentWallHug::UnPeek(UCameraComponent *FollowCamera, PeekDirection Direction, double time)
{
	time += this->GetWorld()->GetDeltaSeconds() * UComponentWallHug::CameraPeekSpeed;
	double target = (Direction == PeekDirection::LEFT ? UComponentWallHug::CameraMaxOffSetX : -UComponentWallHug::CameraMaxOffSetX);
	if (time >= 1)
	{
		FollowCamera->SetRelativeLocation(FVector{0.0f});
		return;
	}
	FollowCamera->SetRelativeLocation(FVector{
		0.0f,
		UKismetMathLibrary::FInterpEaseInOut(target, 0, time, 2),
		0.0f});
	this->GetWorld()->GetTimerManager().SetTimerForNextTick([this, FollowCamera, Direction, time]()
															{ this->UnPeek(FollowCamera, Direction, time); });
}

void UComponentWallHug::HandlePeekLook(FVector2d LookAxisVector)
{
	FVector controllerRotation = this->MyOwner->GetControlRotation().Vector();
	FVector forwardVector = this->MyOwner->GetActorForwardVector();
	FVector rightVector = this->MyOwner->GetActorRotation().RotateVector(FVector::RightVector);
	MyOwner->AddControllerYawInput(LookAxisVector.X);
	MyOwner->AddControllerPitchInput(LookAxisVector.Y);
	if (FVector::DotProduct(forwardVector, controllerRotation) > 0)
	{
		if (FVector::DotProduct(controllerRotation, rightVector) > 0)
		{
			FRotator newRot = this->MyOwner->GetController()->GetControlRotation();
			newRot.Yaw = rightVector.Rotation().Yaw;
			this->MyOwner->GetController()->SetControlRotation(newRot);
		}
		else
		{
			FRotator newRot = this->MyOwner->GetController()->GetControlRotation();
			newRot.Yaw = (-rightVector).Rotation().Yaw;
			this->MyOwner->GetController()->SetControlRotation(newRot);
		}
	}
}

void UComponentWallHug::UpdateIsHuggingWall(bool state)
{
	this->IsHuggingWall = state;
	this->MyOwner->GetCharacterMovement()->bOrientRotationToMovement = !state;
	if (!state)
	{
		this->ZoomOut(this->MyOwner->GetCameraBoom());
		if (this->PeekState != PeekDirection::NONE)
		{
			this->UnPeek(this->MyOwner->GetFollowCamera(), this->PeekState);
			this->PeekState = PeekDirection::NONE;
		}
		// this->ResetCamera();
	}
}