// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"

#include "GASP8Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsFallingUpdated, bool, IsFalling);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsShouldMoveUpdated, bool, IsFalling);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuardWeigthUpdated, float, NewWeight);

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class AGASP8Character : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent *FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext *DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *LookAction;

public:
	AGASP8Character();

	UAbilitySystemComponent *AbilitySystemComponent;
	class UComponentLockon *MyLockonComponent;
	class UComponentWallHug *MyWallHugComponent;
	FGenericTeamId PlayerTeam;

	UPROPERTY(BlueprintAssignable, Category = "AnimState")
	FOnIsFallingUpdated NotifyIsFallingChange;
	UPROPERTY(BlueprintAssignable, Category = "AnimState")
	FOnIsShouldMoveUpdated NotifyShouldMoveChange;
	UPROPERTY(BlueprintAssignable, Category = "AnimState")
	FOnGuardWeigthUpdated NotifyGuardWeigthChange;
	float GuardWeight;
	bool IsGuarding;
	bool IsHuggingWall;

	inline void SetupMyComponents();

	void Tick(float DeltaSeconds);

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool ShouldMove;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;
	static FGameplayTagContainer DisableMovementTags;

	UFUNCTION()
	void HandleGuardEvent(bool IsGuarding);

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = (uint8)0U) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue &Value);

	/** Called for looking input */
	void Look(const FInputActionValue &Value);

	void StopMoving();

protected:
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent *GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent *GetFollowCamera() const { return FollowCamera; }
	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override { return this->AbilitySystemComponent; }

	UFUNCTION(BlueprintCallable)
	virtual inline void SetGenericTeamId(const FGenericTeamId& TeamID) override { this->PlayerTeam = TeamID; };
	UFUNCTION(BlueprintCallable)
	virtual inline FGenericTeamId GetGenericTeamId() const override { return this->PlayerTeam; };
};
