// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Animation/DDPlayEffectAnimNotify.h"

void UDDPlayEffectAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnNotified.Broadcast();
}
