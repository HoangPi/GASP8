// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASP8Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "MyComponents/Movement/ComponentSprint.h"
#include "MyComponents/Movement/ComponentWallHug.h"
#include "MyComponents/Combat/Lockon/ComponentLockon.h"
#include "MyComponents/Combat/Guard/ComponentGuard.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyTags/MyTags.h"
#include "MyAttributes/Health/AttributeHealth.h"
#include "MyAbilities/Combat/Guard/AbilityGuard.h"
#include "Ultilities/TeamEnum.h"
#include "Ultilities/GobalVars.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGASP8Character

FGameplayTagContainer AGASP8Character::DisableMovementTags;

AGASP8Character::AGASP8Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;			 // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 230.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;		// The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;								// Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// #region SetUpDefaultProperties 
	// #tag Animation_state
	this->IsGuarding = false;
	this->GuardWeight = 0.0f;
	this->IsHuggingWall = false;
	
	AGASP8Character::DisableMovementTags.AddTag(Tags::PlayerState::on_air);
	AGASP8Character::DisableMovementTags.AddTag(Tags::PlayerState::disabled);
	
	// #tag Ability_System_Component
	this->AbilitySystemComponent = this->CreateDefaultSubobject<UAbilitySystemComponent>(FName("MCAbilitySystemComponent"));
	// #tag Components
	this->SetupMyComponents();
	// #tag Team
	this->SetGenericTeamId((uint8)ETeamEnum::Player);
	// #endregion
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGASP8Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGASP8Character::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGASP8Character::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AGASP8Character::StopMoving);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGASP8Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGASP8Character::Move(const FInputActionValue &Value)
{
	// input is a Vector2D
	if (this->AbilitySystemComponent->HasMatchingGameplayTag(Tags::PlayerState::disabled))
	{
		return;
	}
	if(this->AbilitySystemComponent->HasMatchingGameplayTag(Tags::PlayerState::on_air))
	{
		AddMovementInput(this->GetActorRotation().Vector(), 1);
		return;
	}
	FVector2D MovementVector = Value.Get<FVector2D>();

	
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		if(this->IsHuggingWall)
		{
			FVector camRot = this->GetFollowCamera()->GetForwardVector();
			camRot.Z = 0;
			camRot.Normalize();
			FVector target = {MovementVector.X, MovementVector.Y, 0.0f};
			target.Normalize();
			this->MyWallHugComponent->WallHugMovement(FVector::DotProduct(camRot, target) < 0);
		}
		else
		{
			// add movement
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}

		// Update movement information
		this->Velocity = this->GetCharacterMovement()->Velocity;
		this->GroundSpeed = FVector(Velocity.X, Velocity.Y, 0).Length();

		// Check if player is "moving"
		if (
			this->GroundSpeed > 30 &&
			UKismetMathLibrary::NotEqual_VectorVector(this->GetCharacterMovement()->GetCurrentAcceleration(), FVector(0), 0))
		{
			if (!this->ShouldMove)
			{
				this->ShouldMove = true;
				this->NotifyShouldMoveChange.Broadcast(true);
				this->AbilitySystemComponent->AddLooseGameplayTag(Tags::PlayerState::should_move);
			}
		}
		else if (this->ShouldMove)
		{
			this->ShouldMove = false;
			this->NotifyShouldMoveChange.Broadcast(false);
			this->AbilitySystemComponent->RemoveLooseGameplayTag(Tags::PlayerState::should_move);
		}
	}
}

void AGASP8Character::Look(const FInputActionValue &Value)
{
	// input is a Vector2D
	if (this->MyLockonComponent->LockonTarget)
	{
		return;
	}
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGASP8Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (this->IsGuarding)
	{
		if (this->GuardWeight < 1.0f)
		{
			float newWeigth = this->GuardWeight + DeltaSeconds * Animation::GuardBlendSpeed;
			this->GuardWeight = (newWeigth <= 1.0f ? newWeigth : 1.0f);
			this->NotifyGuardWeigthChange.Broadcast(this->GuardWeight);
		}
		else
		{
			this->SetActorTickEnabled(false);
		}
	}
	else if (this->GuardWeight > 0.0f)
	{
		float newWeigth = this->GuardWeight - DeltaSeconds * Animation::GuardBlendSpeed;
		this->GuardWeight = (newWeigth >= 0.0f ? newWeigth : 0.0f);
		this->NotifyGuardWeigthChange.Broadcast(this->GuardWeight);
	}
	else
	{
		this->SetActorTickEnabled(false);
	}
}

void AGASP8Character::SetupMyComponents()
{
	this->CreateDefaultSubobject<UComponentSprint>(FName("MovementComponent"));
	this->MyWallHugComponent = this->CreateDefaultSubobject<UComponentWallHug>(FName("WallHugComponent"));
	UComponentGuard *tempGuardComponent = this->CreateDefaultSubobject<UComponentGuard>(FName("MyGuardComponent"));
	UAttributeHealth *healthAttribute = this->CreateDefaultSubobject<UAttributeHealth>(FName("HealthAttribute"));
	this->MyLockonComponent = this->CreateDefaultSubobject<UComponentLockon>(FName("LockonComponent"));
	this->AbilitySystemComponent->AddAttributeSetSubobject(healthAttribute);

	((UAbilityGuard *)this->AbilitySystemComponent->FindAbilitySpecFromClass(UAbilityGuard::StaticClass())->GetPrimaryInstance())->NotifyPlayerGuard.AddDynamic(this, &AGASP8Character::HandleGuardEvent);
}

void AGASP8Character::HandleGuardEvent(bool newGuard)
{
	this->IsGuarding = newGuard;
	this->SetActorTickEnabled(true);
}

void AGASP8Character::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	// I got paranoid that this might overwrite the previous state
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if (PrevMovementMode == EMovementMode::MOVE_Falling)
	{
		this->NotifyIsFallingChange.Broadcast(false);
		this->AbilitySystemComponent->RemoveLooseGameplayTag(Tags::PlayerState::on_air);
	}
	else if (this->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		this->Velocity = this->GetCharacterMovement()->Velocity;
		this->NotifyIsFallingChange.Broadcast(true);
		this->AbilitySystemComponent->AddLooseGameplayTag(Tags::PlayerState::on_air);
	}
}

void AGASP8Character::StopMoving()
{
	this->ShouldMove = false;
	this->NotifyShouldMoveChange.Broadcast(false);
	this->AbilitySystemComponent->RemoveLooseGameplayTag(Tags::PlayerState::should_move);
}