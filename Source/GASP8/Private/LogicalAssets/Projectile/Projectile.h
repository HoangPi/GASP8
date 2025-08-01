// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

#include "Projectile.generated.h"

UCLASS(Blueprintable)
class AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(EditDefaultsOnly, Category = "Property")
	UBoxComponent *CollisionBox;

	UPROPERTY(EditDefaultsOnly, Category = "Property")
	UArrowComponent *ForwardArrow;

	UPROPERTY(EditDefaultsOnly, Category = "Property")
	float Speed;
	UPROPERTY(EditDefaultsOnly, Category = "Property")
	float MaxTurningSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Property")
	bool ShouldHaveTarget;
	UPROPERTY(EditDefaultsOnly, Category = "Property")
	float TimeToLive;
	AActor *MyTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
