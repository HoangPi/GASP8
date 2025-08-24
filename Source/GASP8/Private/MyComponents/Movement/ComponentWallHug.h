// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ComponentWallHug.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UComponentWallHug : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponentWallHug();

	UInputAction *InteractAction;
	// When you modify this bool, do perform this side effect ResetCamera
	bool IsHuggingWall = false;

	void WallHug();
	void WallHugMovement(bool IsMovingLeft = true);
	class AGASP8Character *MyOwner;
	void ResetCamera();
	inline void UpdateIsHuggingWall(bool state);

	static double CameraPeekSpeed;
	static double CameraMaxPeekDistance;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
