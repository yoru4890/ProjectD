// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Trap/DDTrapEffectAnimNotify.h"
#include "LSM/Building/Trap/DDTrapAttackEffectInterface.h"

void UDDTrapEffectAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        // 액터가 IDDTrapAttackInterface를 구현하고 있는지 확인
        IDDTrapAttackEffectInterface* AttackEffectInterface = Cast<IDDTrapAttackEffectInterface>(MeshComp->GetOwner());
        UE_LOG(LogTemp, Warning, TEXT("attack notify"));
        if (AttackEffectInterface)
        {
            UE_LOG(LogTemp, Warning, TEXT("AttackInterface is not null"));
            // 인터페이스의 함수를 호출하여 공격 실행
            AttackEffectInterface->StartAttackEffect();
        }
    }
}