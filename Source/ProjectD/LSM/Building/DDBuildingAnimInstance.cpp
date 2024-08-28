// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/DDBuildingAnimInstance.h"

UDDBuildingAnimInstance::UDDBuildingAnimInstance()
{

}

void UDDBuildingAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
    SkeletalMeshComponent = GetOwningComponent();
}

void UDDBuildingAnimInstance::PlayAnimationMontage(UAnimMontage* InMontage)
{
    if (InMontage)
    {
        if (Montage_Play(InMontage) > 0)
        {
            CurrentMontage = InMontage;

            // ���� ��Ÿ�ְ� ������ bIsAttacking�� false�� ����
            FOnMontageEnded MontageEndedDelegate;
            MontageEndedDelegate.BindUObject(this, &UDDBuildingAnimInstance::OnMontageEnded);
            Montage_SetEndDelegate(MontageEndedDelegate, InMontage);
        }
        else
        {
            //UE_LOG(LogTemp, Error, TEXT("Failed to play montage"));
        }
    }
    else
    {
        //UE_LOG(LogTemp, Error, TEXT("InMontage is null"));
    }
}

void UDDBuildingAnimInstance::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == CurrentMontage)
    {
        CurrentMontage = nullptr;
    }
}
