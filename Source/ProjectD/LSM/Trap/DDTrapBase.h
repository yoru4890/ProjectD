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
	FString TrapDisplayName; // 트랩의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TrapRowName; // 트랩의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAttack; // 트랩이 공격할 수 있는지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapBuildCost; // 트랩의 설치비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapUpgradeCost; // 트랩의 업그레이드 비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapUnlockCost; // 트랩의 언락 비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapCoolTime; // 트랩의 공격 쿨타임

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapDamage; // 트랩의 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapLevel; // 트랩의 레벨(테크)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TrapParentName; // 부모 트랩의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> TrapChildNames; // 자식 트랩의 이름들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTrapUnlocked; // 트랩이 언락되었는지 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMeshType TrapMeshType; // 트랩의 메쉬 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDotTrap; // 도트 공격 트랩 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotDamage; // 도트 공격 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotDuration; // 도트 공격 지속 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotInterval; // 도트 공격 간격

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSlowTrap; // 이동 속도 감소 트랩 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlowTrap"))
	float SlowAmount; // 이동 속도 감소 비율 (0.0 ~ 1.0, 예: 0.5는 50% 감소)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlowTrap"))
	float SlowDuration; // 이동 속도 감소 지속 시간

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
