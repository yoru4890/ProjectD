// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDTrapStruct.h"
#include "TrapAssetInterface.h"
#include "DDTrapBase.generated.h"


USTRUCT(BlueprintType)
struct FMaterialsStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> Materials;
};

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
	FString TrapDisplayName; // Ʈ���� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TrapRowName; // Ʈ���� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAttack; // Ʈ���� ������ �� �ִ���

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
	EMeshType TrapMeshType; // Ʈ���� �޽� Ÿ��

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
	TArray<TObjectPtr<UParticleSystemComponent>> ParticleEffectComponents;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BoxCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TObjectPtr<UMaterialInterface> PreviewMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TMap<int32, FMaterialsStruct> OriginalMaterials;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

public:
	FORCEINLINE const FName& GetTrapRowName() const { return TrapRowName; }
	FORCEINLINE const int32 GetTrapBuildCost() const { return TrapBuildCost; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InitFromDataTable(const FName& RowName, const FDDTrapStruct& TrapData);
	virtual void SetTrapAssets(TArray<UStaticMesh*> StaticMeshs, TArray<USkeletalMesh*> SkeletalMeshs, UAnimBlueprint* AnimBlueprint, TArray<UParticleSystem*> ParticleEffects) override;
	void SetMaterialToPreview(bool bCanPay);
	void SetMaterialToOriginal();
};
