// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Animation/AttackTraceAnimNotify.h"

void UAttackTraceAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	OnNotified.Broadcast();
}
