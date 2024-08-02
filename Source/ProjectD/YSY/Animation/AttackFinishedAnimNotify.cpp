// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Animation/AttackFinishedAnimNotify.h"

void UAttackFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	onNotified.Broadcast();
}
