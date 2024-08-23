// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DDPlayerAttackComboNotifyState.generated.h"

UCLASS()
class PROJECTD_API UDDPlayerAttackComboNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()


	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName SectionIndex;

private:
	FName* WeaponCompSectionIndex;
};
