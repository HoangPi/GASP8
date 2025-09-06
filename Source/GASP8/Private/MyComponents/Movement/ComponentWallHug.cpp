// Fill out your copyright notice in the Description page of Project Settings.

#include "MyComponents/Movement/ComponentWallHug.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GASP8/GASP8Character.h"
#include "Ultilities/Macro.h"

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
		AController *controller = this->MyOwner->GetController();
		FRotator rot = controller->GetControlRotation();
		this->UnPeek(
			controller,
			rot,
			UKismetMathLibrary::NormalizedDeltaRotator(
				this->MyOwner->GetActorRotation().RotateVector(FVector::BackwardVector).Rotation(),
				rot),
			this->MyOwner->GetFollowCamera()->GetRelativeRotation());
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
		// DeltaControlRotation.Pitch = 0.0f;
		constexpr double YOffset = 65.0f;
		const FRotator initControlRotation = this->MyOwner->GetControlRotation();
		this->PeekState = (IsMovingRight ? PeekDirection::RIGHT : PeekDirection::LEFT);
		this->Peek(
			{initControlRotation.Pitch, UKismetMathLibrary::NormalizedDeltaRotator(FRotator::ZeroRotator, initControlRotation).Pitch},
			this->MyOwner->GetCameraBoom(), 
			this->MyOwner->GetFollowCamera(), 
			IsMovingRight ? -YOffset : YOffset);
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

void UComponentWallHug::Peek(const StatePeristedObj<double> ControlPitchRotation, USpringArmComponent *CameraBoom, UCameraComponent *FollowCamera, double YOffset, double time)
{
	constexpr double XOffset = -100.0f;
	constexpr double ZOffset = 60.0f;
	time += this->GetWorld()->GetDeltaSeconds() * UComponentWallHug::CameraPeekSpeed;
	if (time >= 1)
	{
		CameraBoom->TargetArmLength = 0;
		auto rot = this->MyOwner->GetControlRotation();
		rot.Pitch = 0.0f;
		this->MyOwner->GetController()->SetControlRotation(rot);
		FollowCamera->SetRelativeLocation(FVector({XOffset, YOffset, ZOffset}));
		return;
	}
	// Interp this faster before camera boom's change take full effect (snappy)
	if(const double axlrtime = time * 1.35 < 1)
	{
		FRotator controlRotation = this->MyOwner->GetControlRotation();
		controlRotation.Pitch = ControlPitchRotation.OldObj + UKismetMathLibrary::FInterpTo(0, ControlPitchRotation.NewObj, axlrtime, 1);
		this->MyOwner->GetController()->SetControlRotation(controlRotation);
		FollowCamera->SetRelativeLocation(UKismetMathLibrary::VInterpTo(FVector::ZeroVector, FVector(XOffset, YOffset, ZOffset), axlrtime, 1));
	}
	CameraBoom->TargetArmLength = UKismetMathLibrary::FInterpTo(CameraBoom->TargetArmLength, 0, time, 1);
	this->GetWorld()->GetTimerManager().SetTimerForNextTick([this, ControlPitchRotation, CameraBoom, FollowCamera, YOffset, time]()
															{ this->Peek(ControlPitchRotation, CameraBoom, FollowCamera, YOffset, time); });
}
void UComponentWallHug::UnPeek(AController *Controller, FRotator InitControlRotation, FRotator DeltaControlRotation, FRotator InitRelativeRotation, double time)
{
	time += this->GetWorld()->GetDeltaSeconds() * UComponentWallHug::CameraPeekSpeed;
	if (time >= 1)
	{
		InitControlRotation.Add(DeltaControlRotation.Pitch, DeltaControlRotation.Yaw, 0.0f);
		Controller->SetControlRotation(InitControlRotation);
		this->MyOwner->GetFollowCamera()->SetRelativeLocationAndRotation(
			FVector(0.0f),
			FRotator{0.0f});
		this->MyOwner->GetCameraBoom()->TargetArmLength = this->OriginCameraLength - UComponentWallHug::CameraMaxOffSetY + UComponentWallHug::PeekCameraLengthOffset;
		return;
	}
	const double deltaPitchhRotation = UKismetMathLibrary::FInterpEaseInOut(0, DeltaControlRotation.Pitch, time, 2);
	const double deltaYawRotation = UKismetMathLibrary::FInterpEaseInOut(0, DeltaControlRotation.Yaw, time, 2);
	FRotator currentRotaion = InitControlRotation;
	currentRotaion.Add(deltaPitchhRotation, deltaYawRotation, 0.0f);
	FRotator currentRelativeRotation({UKismetMathLibrary::FInterpEaseInOut(InitRelativeRotation.Pitch, 0, time, 2),
									  UKismetMathLibrary::FInterpEaseInOut(InitRelativeRotation.Yaw, 0, time, 2),
									  0.0f});
	FVector relativeLocation{0.0f, 0.0f, UKismetMathLibrary::FInterpEaseInOut(UComponentWallHug::CameraMaxOffSetX, 0, time, 2)};
	this->MyOwner->GetFollowCamera()->SetRelativeLocationAndRotation(relativeLocation, currentRelativeRotation);
	this->MyOwner->GetCameraBoom()->TargetArmLength = this->OriginCameraLength - UComponentWallHug::CameraMaxOffSetY + UKismetMathLibrary::FInterpEaseInOut(UComponentWallHug::PeekCameraLengthOffset, 0, time, 2);
	Controller->SetControlRotation(currentRotaion);
	this->GetWorld()->GetTimerManager().SetTimerForNextTick([this, Controller, InitControlRotation, DeltaControlRotation, InitRelativeRotation, time]()
															{ this->UnPeek(Controller, InitControlRotation, DeltaControlRotation, InitRelativeRotation, time); });
}

void UComponentWallHug::HandlePeekLook(FVector2d LookAxisVector)
{
	FVector rightVector = this->MyOwner->GetActorRotation().RotateVector(FVector::RightVector);
	FVector forwardVector = this->MyOwner->GetActorForwardVector();
	if (this->PeekState != PeekDirection::NONE)
	{
		FRotator relativeRotation = this->MyOwner->GetFollowCamera()->GetRelativeRotation();
		relativeRotation.Pitch -= LookAxisVector.Y;
		relativeRotation.Pitch = CLAMP(relativeRotation.Pitch, -45.0f, 45.0f);
		relativeRotation.Yaw += LookAxisVector.X;
		const bool left = this->PeekState == PeekDirection::LEFT;
		const double min = left ? 0 : -179.0f;
		const double max = left ? 179.0f : 0;
		relativeRotation.Yaw = CLAMP(relativeRotation.Yaw, min, max);
		this->MyOwner->GetFollowCamera()->SetRelativeRotation(relativeRotation);
		return;
	}
	FVector controllerRotation = this->MyOwner->GetControlRotation().Vector();
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
			AController *controller = this->MyOwner->GetController();
			FRotator rot = controller->GetControlRotation();
			this->UnPeek(
				controller,
				rot,
				UKismetMathLibrary::NormalizedDeltaRotator(
					this->MyOwner->GetActorRotation().RotateVector(FVector::BackwardVector).Rotation(),
					rot),
				this->MyOwner->GetFollowCamera()->GetRelativeRotation());
			this->PeekState = PeekDirection::NONE;
		}
		// this->ResetCamera();
	}
}