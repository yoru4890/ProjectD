// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Animation/DDCudgelCollisionNotifyState.h"
#include "LJW/Interface/DDPlayerComponentsAnimInterface.h"
#include "LJW/Weapon/DDWeaponSystemComponent.h"

void UDDCudgelCollisionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UE_LOG(LogTemp,Warning,TEXT("Cudgel Collision QueryOnly changed"))
	//Collision On
	auto PlayerComp = Cast<IDDPlayerComponentsAnimInterface>(MeshComp->GetOwner());

	if (PlayerComp)
	{
		CudgelCollision = PlayerComp->GetWeaponComp()->CudgelCollision;
		CudgelCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UDDCudgelCollisionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//Collision Off
	if (CudgelCollision)
	{
		CudgelCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
