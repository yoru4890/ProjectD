// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/DDBuildingAttackAnimNotify.h"
#include "LSM/Building/DDBuildingAttackInterface.h"

void UDDBuildingAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        // ���Ͱ� IDDTrapAttackInterface�� �����ϰ� �ִ��� Ȯ��
        IDDBuildingAttackInterface* AttackInterface = Cast<IDDBuildingAttackInterface>(MeshComp->GetOwner());
        UE_LOG(LogTemp, Warning, TEXT("attack notify"));
        if (AttackInterface)
        {
            UE_LOG(LogTemp, Warning, TEXT("AttackInterface is not null"));
            // �������̽��� �Լ��� ȣ���Ͽ� ���� ����
            AttackInterface->Attack();
        }
    }
}
