// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Trap/DDTrapAnimInstance.h"

UDDTrapAnimInstance::UDDTrapAnimInstance()
{

}

void UDDTrapAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UDDTrapAnimInstance::PlayAnimationMontage(UAnimMontage* InMontage)
{
    if (InMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Trap Attack Montage Exist"));
        if (Montage_Play(InMontage) > 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Montage is playing successfully"));
            CurrentMontage = InMontage;

            // 공격 몽타주가 끝나면 bIsAttacking을 false로 설정
            FOnMontageEnded MontageEndedDelegate;
            MontageEndedDelegate.BindUObject(this, &UDDTrapAnimInstance::OnMontageEnded);
            Montage_SetEndDelegate(MontageEndedDelegate, InMontage);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to play montage"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("InMontage is null"));
    }
}

void UDDTrapAnimInstance::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == CurrentMontage)
    {
        CurrentMontage = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("Montage ended: %s"), bInterrupted ? TEXT("Interrupted") : TEXT("Completed"));
    }
}
