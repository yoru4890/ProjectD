// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackFinishedNotifySignature);

UCLASS()
class PROJECTD_API UAttackFinishedAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FOnAttackFinishedNotifySignature onNotified;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
