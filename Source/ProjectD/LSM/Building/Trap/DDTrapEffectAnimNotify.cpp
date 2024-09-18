// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Trap/DDTrapEffectAnimNotify.h"
#include "LSM/Building/Trap/DDTrapAttackEffectInterface.h"

void UDDTrapEffectAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        // ���Ͱ� IDDTrapAttackInterface�� �����ϰ� �ִ��� Ȯ��
        IDDTrapAttackEffectInterface* AttackEffectInterface = Cast<IDDTrapAttackEffectInterface>(MeshComp->GetOwner());
        UE_LOG(LogTemp, Warning, TEXT("attack notify"));
        if (AttackEffectInterface)
        {
            UE_LOG(LogTemp, Warning, TEXT("AttackInterface is not null"));
            // �������̽��� �Լ��� ȣ���Ͽ� ���� ����
            AttackEffectInterface->StartAttackEffect();
        }
    }
}