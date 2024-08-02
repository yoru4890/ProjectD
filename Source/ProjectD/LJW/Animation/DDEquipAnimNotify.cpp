// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Animation/DDEquipAnimNotify.h"

void UDDEquipAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	OnEquipDelegatge.Broadcast();
}

