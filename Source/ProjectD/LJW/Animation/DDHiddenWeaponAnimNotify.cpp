// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Animation/DDHiddenWeaponAnimNotify.h"

void UDDHiddenWeaponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnHiddenDelegatge.Broadcast();
}
