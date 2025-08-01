// Fill out your copyright notice in the Description page of Project Settings.

#include "LogicalAssets/Projectile/Projectile.h"

#include "Ultilities/Macro.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->CollisionBox = this->CreateDefaultSubobject<UBoxComponent>(FName("ProjectileCollisionBox"));
	this->RootComponent = this->CollisionBox;
	this->ForwardArrow = this->CreateDefaultSubobject<UArrowComponent>(FName("ForwarArrow"));
	this->ForwardArrow->SetupAttachment(this->CollisionBox);
	this->ForwardArrow->SetWorldRotation(this->GetActorRotation());

	this->Speed = 500.0f;
	this->MaxTurningSpeed = 24.0f;
	this->TimeToLive = 3.0f;
	this->ShouldHaveTarget = false;
	this->MyTarget = nullptr;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator currentRot = this->GetActorRotation();
	if (this->ShouldHaveTarget)
	{
		if (!this->MyTarget)
		{
			return;
		}
		FRotator dif = UKismetMathLibrary::NormalizedDeltaRotator(currentRot, UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), this->MyTarget->GetActorLocation()));
		double max = this->MaxTurningSpeed * DeltaTime;
		double min = -max;
		currentRot.Pitch += CLAMP(dif.Pitch, min, max);
		currentRot.Yaw += CLAMP(dif.Yaw, min, max);
	}
	this->SetActorLocationAndRotation(
		this->GetActorLocation() + currentRot.Vector() * this->Speed * DeltaTime,
		FQuat(currentRot),
		true,
		nullptr,
		ETeleportType::None);
}
