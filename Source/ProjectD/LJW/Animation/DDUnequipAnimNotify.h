// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DDUnequipAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnUnequipSignature);

UCLASS()
class PROJECTD_API UDDUnequipAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnUnequipSignature OnUnequipDelegatge;
};
