// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ultilities/Template.h"

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
	double OriginCameraLength;
	const double PeekCameraLengthOffset = 75.0f;
	const double CameraMaxOffSetY = 180.0f;
	class AGASP8Character *MyOwner;
	const double CameraPeekSpeed = 1.8f;
	static FCollisionObjectQueryParams TraceObjects;
	static FCollisionQueryParams ActorsToIgnores;
	enum PeekDirection : uint8
	{
		NONE = 0,
		LEFT = 1,
		RIGHT = 2
	};
	PeekDirection PeekState;

	void WallHug();
	void WallHugMovement(bool IsMovingRight = true);
	void UpdateIsHuggingWall(bool state);
	void ZoomIn(USpringArmComponent *SpringArm, double time = 0.0f);
	void ZoomOut(USpringArmComponent *SpringArm, double time = 0.0f);
	void Peek(USpringArmComponent *CameraBoom, const double YOffset, double time = 0.0f);
	void UnPeek(USpringArmComponent *CameraBoom, FVector InitCameraLocation, double time = 0.0f);
	void HandlePeekLook(FVector2d LookAxisVector);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
