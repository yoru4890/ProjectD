// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDTrapStruct.h"
#include "TrapAssetInterface.h"
#include "DDTrapBase.generated.h"

UCLASS()
class PROJECTD_API ADDTrapBase : public AActor, public ITrapAssetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDTrapBase();
	virtual ~ADDTrapBase() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TrapName; // Ʈ���� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETrapType TrapType; // Ʈ���� �޽� Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapBuildCost; // Ʈ���� ��ġ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapUpgradeCost; // Ʈ���� ���׷��̵� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapUnlockCost; // Ʈ���� ��� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapCoolTime; // Ʈ���� ���� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapDamage; // Ʈ���� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapLevel; // Ʈ���� ����(��ũ)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TrapParentName; // �θ� Ʈ���� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> TrapChildNames; // �ڽ� Ʈ���� �̸���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTrapUnlocked; // Ʈ���� ����Ǿ����� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETrapMeshType TrapMeshType; // Ʈ���� �޽� Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "TrapMeshType == ETrapMeshType::StaticMesh"))
	TObjectPtr<UStaticMesh> TrapStaticMesh; // Ʈ���� ����ƽ �޽�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "TrapMeshType == ETrapMeshType::SkeletalMesh"))
	TObjectPtr<USkeletalMesh> TrapSkeletalMesh; // Ʈ���� ���̷��� �޽�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "TrapMeshType == ETrapMeshType::SkeletalMesh"))
	TObjectPtr<UAnimBlueprint> TrapAnimBlueprint; // Ʈ���� �ִϸ��̼� �������Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> TrapEffect; // Ʈ���� ���� ����Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDotTrap; // ��Ʈ ���� Ʈ�� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotDamage; // ��Ʈ ���� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotDuration; // ��Ʈ ���� ���� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotInterval; // ��Ʈ ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSlowTrap; // �̵� �ӵ� ���� Ʈ�� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlowTrap"))
	float SlowAmount; // �̵� �ӵ� ���� ���� (0.0 ~ 1.0, ��: 0.5�� 50% ����)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlowTrap"))
	float SlowDuration; // �̵� �ӵ� ���� ���� �ð�

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystemComponent> ParticleEffectComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InitFromDataTable(const FDDTrapStruct& TrapData);
	virtual void SetTrapAssets(UStaticMesh* StaticMesh, USkeletalMesh* SkeletalMesh, UAnimBlueprint* AnimBlueprint, UParticleSystem* ParticleEffect) override;

};
