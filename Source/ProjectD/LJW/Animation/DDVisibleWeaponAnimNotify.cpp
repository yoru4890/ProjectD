// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Animation/DDVisibleWeaponAnimNotify.h"

void UDDVisibleWeaponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnVisibleDelegatge.Broadcast();
}
