// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YSY/Interface/DamageInterface.h"
#include "DDBuildingAttackStrategyInterface.h"
#include "DDBaseAttackStrategy.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTD_API UDDBaseAttackStrategy : public UObject, public IDDBuildingAttackStrategyInterface
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(class ADDBuildingBase* InOwningTower);

	virtual void Attack(AActor* TargetEnemy, UStaticMeshComponent* FireStaticMesh) override;

protected:
	void ApplyDotDamge(AActor* TargetEnemy);
	void ApplyDirectDamage(AActor* TargetEnemy);
	void ApplyDebuff(AActor* TargetEnemy);

	void PlayHitEffect(AActor* TargetEnemy, FVector Location = FVector(), FRotator Rotation = FRotator(), FName SocketName = NAME_None, bool bAttachToTarget = true);

protected:
	UPROPERTY()
	TObjectPtr<class ADDBuildingBase> OwningTower;

	TSubclassOf<UDamageType> DamageType;

	float Damage;

	bool bIsDot;

	EDotDamageType DotDamageType;

	float DotDamage; // 도트 공격 데미지

	float DotDuration; // 도트 공격 지속 시간

	float DotInterval; // 도트 공격 간격

	bool bIsDebuff;

	EDebuffType DebuffType;

	float DebuffRate; // 이동 속도 감소 비율 (0.0 ~ 1.0, 예: 0.5는 50% 감소)

	float DebuffDuration; // 이동 속도 감소 지속 시간
};
