// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DDEquipAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEquipSignature);

UCLASS()
class PROJECTD_API UDDEquipAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnEquipSignature OnEquipDelegatge;
};
