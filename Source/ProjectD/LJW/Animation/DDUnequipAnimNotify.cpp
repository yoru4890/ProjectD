// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Animation/DDUnequipAnimNotify.h"

void UDDUnequipAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	OnUnequipDelegatge.Broadcast();
}
