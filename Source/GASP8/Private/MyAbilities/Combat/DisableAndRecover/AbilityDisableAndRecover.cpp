// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAbilities/Combat/DisableAndRecover/AbilityDisableAndRecover.h"

#include "MyTags/MyTags.h"
#include "LogicalAssets/Abilities/AbilityBase.h"
#include "MyEffects/PlayerState/EffectDisable.h"

UAbilityDisableAndRecover::UAbilityDisableAndRecover()
{
    this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
    this->ActivationOwnedTags.AddTag(Tags::PlayerState::disabled);

    this->FallDownAndRecoverMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/ThirdPerson/Anim/FallDown_Montage.FallDown_Montage"));
    this->StandUpMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/ThirdPerson/Anim/StandingUp_Montage.StandingUp_Montage"));

    this->OnMontageCompletedDelegate.BindUFunction(this, FName("NotifyMontageCompleted"));
    this->OnDisableEnd.BindUFunction(this, FName("NotifyDisableEffectEnded"));

    this->CanRecover = false;
}

void UAbilityDisableAndRecover::ActivateAbility(
    FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    UAbilitySystemComponent *ownerASC = ActorInfo->AbilitySystemComponent.Get();
    ownerASC->CancelAbilities(&UAbilityBase::CancelOnDisableTags);

    UAbilityTask_WaitGameplayEffectRemoved *waitQuickRecover =
        UAbilityTask_WaitGameplayEffectRemoved::WaitForGameplayEffectRemoved(
            this,
            ownerASC->ApplyGameplayEffectToSelf((UEffectDisable *)UEffectDisable::StaticClass()->GetDefaultObject(), 1.0f, ownerASC->MakeEffectContext()));
    waitQuickRecover->OnRemoved.Add(this->OnDisableEnd);
    waitQuickRecover->ReadyForActivation();

    UAbilityTask_PlayMontageAndWait *animTask1 = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this,
        FName("KnockPlayerDown"),
        this->FallDownAndRecoverMontage,
        1.0f,
        FName(NAME_None),
        false,
        1.0f,
        0.0f,
        true);
    animTask1->OnCompleted.Add(this->OnMontageCompletedDelegate);
    animTask1->ReadyForActivation();
}

void UAbilityDisableAndRecover::CancelAbility(
    FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateCancelAbility)
{
    if(!this->CanRecover)
    {
        return;
    }
    UAbilityTask_PlayMontageAndWait *animTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this,
        FName("KnockPlayerDown"),
        this->StandUpMontage,
        1.0f,
        FName(NAME_None),
        false,
        1.0f,
        0.0f,
        true);
    animTask->OnCompleted.Add(this->OnMontageCompletedDelegate);
    animTask->ReadyForActivation();
    this->CanRecover = false;
}

void UAbilityDisableAndRecover::NotifyMontageCompleted()
{
    this->EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, true);
}

void UAbilityDisableAndRecover::NotifyDisableEffectEnded()
{
    this->CanRecover = true;
}
