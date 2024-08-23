 // Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Animation/DDPlayerAttackComboNotifyState.h"
#include "LJW/Interface/DDPlayerComponentsAnimInterface.h"
#include "LJW/Weapon/DDWeaponSystemComponent.h"

void UDDPlayerAttackComboNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	auto PlayerAnimInterface = Cast<IDDPlayerComponentsAnimInterface>(MeshComp->GetOwner());

	if (PlayerAnimInterface)
	{
		WeaponCompSectionIndex = &(PlayerAnimInterface->GetWeaponComp()->ComboSectionIndex);
		*WeaponCompSectionIndex = SectionIndex;
	}

}

void UDDPlayerAttackComboNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (WeaponCompSectionIndex)
	{
		*WeaponCompSectionIndex = FName("A");
	}
}
