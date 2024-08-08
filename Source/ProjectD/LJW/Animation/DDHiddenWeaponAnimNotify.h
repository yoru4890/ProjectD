// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DDHiddenWeaponAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHiddenSignature);

UCLASS()
class PROJECTD_API UDDHiddenWeaponAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnHiddenSignature OnHiddenDelegatge;

};
