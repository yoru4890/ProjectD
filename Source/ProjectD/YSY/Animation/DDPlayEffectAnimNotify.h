// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DDPlayEffectAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayEffectNotifySignature);

UCLASS()
class PROJECTD_API UDDPlayEffectAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnPlayEffectNotifySignature OnNotified;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
