// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/DDBuildingAttackAnimNotify.h"
#include "LSM/Building/DDBuildingAttackInterface.h"

void UDDBuildingAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        // 액터가 IDDTrapAttackInterface를 구현하고 있는지 확인
        IDDBuildingAttackInterface* AttackInterface = Cast<IDDBuildingAttackInterface>(MeshComp->GetOwner());
        UE_LOG(LogTemp, Warning, TEXT("attack notify"));
        if (AttackInterface)
        {
            UE_LOG(LogTemp, Warning, TEXT("AttackInterface is not null"));
            // 인터페이스의 함수를 호출하여 공격 실행
            AttackInterface->Attack();
        }
    }
}
