// Fill out your copyright notice in the Description page of Project Settings.

#include "MyComponents/Combat/Lockon/ComponentLockon.h"

#include "GASP8/GASP8Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Ultilities/MyConst.h"
#include "Ultilities/GobalVars.h"
#include "MyEffects/Cooldown/EffectGenericCooldown.h"
#include "MyTags/MyTags.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UComponentLockon::UComponentLockon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	this->SetComponentTickEnabled(false);
	this->LockonTarget = nullptr;
	this->LockonAction = LoadObject<UInputAction>(nullptr, AssetPath::InputAction::LockonAction);
	this->LookAction = LoadObject<UInputAction>(nullptr, AssetPath::InputAction::LookAction);
	if (IAbilitySystemInterface *owner = this->GetOwner<IAbilitySystemInterface>())
	{
		this->OwnerASC = owner->GetAbilitySystemComponent();
		this->cooldownEffect = this->CreateDefaultSubobject<UEffectGenericCooldown>(FName("SwitchCooldown"));
		this->cooldownEffect->SetCooldown(1.0f);
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
	this->CooldownSpec = FGameplayEffectSpec(this->cooldownEffect, this->OwnerASC->MakeEffectContext(), 1.0f);
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
	if (!this->LockonTarget)
	{
		this->SetComponentTickEnabled(false);
		return;
	}
	AController *controller = this->MyOwner->GetController();
	if (!controller->LineOfSightTo(this->LockonTarget, this->MyOwner->GetFollowCamera()->GetComponentLocation()))
	{
		this->LockonTarget = nullptr;
		this->SetComponentTickEnabled(false);
		return;
	}
	FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(this->MyOwner->GetActorLocation(), this->LockonTarget->GetActorLocation());
	targetRot.Pitch -= 20;
	controller->SetControlRotation(
		UKismetMathLibrary::RInterpTo(controller->GetControlRotation(), targetRot, DeltaTime, 10.0f));
}

void UComponentLockon::SetupMyInputs(UEnhancedInputComponent *input)
{
	input->BindAction(this->LockonAction, ETriggerEvent::Started, this, &UComponentLockon::Lockon);
	input->BindAction(this->LookAction, ETriggerEvent::Triggered, this, &UComponentLockon::Switch);
}

void UComponentLockon::Lockon()
{
	if (this->LockonTarget)
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
		AController *controller = this->MyOwner->GetController();
		FRotator target = this->MyOwner->GetActorRotation();
		if (!controller->LineOfSightTo(result.GetActor(), this->MyOwner->GetFollowCamera()->GetComponentLocation()))
		{
			this->GetWorld()->GetTimerManager().SetTimerForNextTick([this, controller, target]()
																	{ this->AdjustCamera(controller, target); });
			return;
		}
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, result.GetActor()->GetName());
		this->LockonTarget = result.GetActor();
		this->SetComponentTickEnabled(true);
		this->CooldownHandle = this->OwnerASC->ApplyGameplayEffectSpecToSelf(this->CooldownSpec);
	}
	else
	{
		AController *controller = this->MyOwner->GetController();
		FRotator target = this->MyOwner->GetActorRotation();
		if (!controller->LineOfSightTo(result.GetActor(), this->MyOwner->GetFollowCamera()->GetComponentLocation()))
		{
			this->GetWorld()->GetTimerManager().SetTimerForNextTick([this, controller, target]()
																	{ this->AdjustCamera(controller, target); });
		}
	}
}

void UComponentLockon::Switch(const FInputActionValue &Value)
{
	if (!this->LockonTarget)
	{
		return;
	}
	float value = Value.Get<FVector2D>().X;
	if (value >= 20.0f || value <= -20.0f)
	{
		if (this->OwnerASC->GetActiveGameplayEffect(this->CooldownHandle))
		{
			return;
		}
		TArray<FHitResult> results;
		FVector start = this->MyOwner->GetFollowCamera()->GetComponentLocation();
		FRotator rot = this->MyOwner->GetFollowCamera()->GetComponentRotation();
		rot.Pitch += 20.0f;
		FVector rotVector = rot.Vector();
		start += rotVector * (this->MyOwner->GetCameraBoom()->TargetArmLength + this->CollisionShape.Box.HalfExtentX);
		start += rot.RotateVector(FVector(0, (value >= 20.0f ? 1 : -1), 0)) * this->CollisionShape.Box.HalfExtentY;
		FVector end = start + rotVector * 3000.0f;
		if (this->GetWorld()->SweepMultiByObjectType(
				results,
				start,
				end,
				FQuat(rot),
				this->CollisionObject,
				this->CollisionShape,
				this->ActorsToIgnore))
		{
			AActor *trueTarget = nullptr;
			double distance = MaxValue::MaxDouble;
			FVector baseLocation = this->LockonTarget->GetActorLocation();
			for (int i = 0; i < (results.Num() <= MaxValue::TraceTarget ? results.Num() : MaxValue::TraceTarget); i++)
			{
				if (results[i].GetActor() == this->LockonTarget)
				{
					continue;
				}
				double test = FVector::Distance(results[i].GetActor()->GetActorLocation(), baseLocation);
				if (test < distance)
				{
					distance = test;
					trueTarget = results[i].GetActor();
				}
			}
			for (int i = 0; i < results.Num(); i++)
			{
				GEngine->AddOnScreenDebugMessage(-1, i / 5, FColor::Black, results[i].GetActor()->GetName());
			}

			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%d targets"), results.Num()));
			if (trueTarget)
			{
				this->LockonTarget = trueTarget;
				this->CooldownHandle = this->OwnerASC->ApplyGameplayEffectSpecToSelf(this->CooldownSpec);
			}
		}
		FHitResult t;
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
			t,
			false,
			FLinearColor::Blue);
	}
}

void UComponentLockon::AdjustCamera(AController *controller, FRotator target, float counter)
{
	counter += this->GetWorld()->GetDeltaSeconds();
	const float speed = 5.0f;
	const float interWeight = counter * speed;
	FRotator result = controller->GetControlRotation();
	result.Yaw = FMath::InterpEaseInOut<float>(controller->GetControlRotation().Yaw, target.Yaw, interWeight, 2.0f);
	controller->SetControlRotation(result);
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("%f"), counter));
	if(interWeight >= 1.0f)
	{
		return;
	}

	this->GetWorld()->GetTimerManager().SetTimerForNextTick([this, controller, counter, target]()
															{ this->AdjustCamera(controller, target, counter); });
}