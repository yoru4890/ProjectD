// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDBuildingBaseData.h"
#include "LSM/DDSetAssetInterface.h"
#include "LSM/DDMaterials.h"
#include "DDBuildingBase.generated.h"

UCLASS()
class PROJECTD_API ADDBuildingBase : public AActor,public IDDSetAssetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDBuildingBase();
	virtual ~ADDBuildingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FORCEINLINE const FName& GetRowName() const { return RowName; }
	FORCEINLINE const int32 GetCellWidth() const { return CellWidth; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InitFromDataTable(const FName& InRowName, const FDDBuildingBaseData& BuildingData);
	void  SetCanAttack(const bool bInCanAttack);
	virtual void SetAssets(FDDBuildingBaseData& LoadedAsset) override;
	void SetMaterialToPreview(bool bCanPay);
	void SetMaterialToOriginal();

protected:
	virtual void Attack();

	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	void SetParticeEffects(FDDBuildingBaseData& LoadedAsset);
	void SetMeshs(FDDBuildingBaseData& LoadedAsset);
	void ModifyScaleAndLocation() const;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName; // 트랩의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName; // 트랩의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CellWidth; // 트랩이 차지하는 셀의 크기

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridCellSize; // 셀 한칸이 차지하는 길이

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAttack; // 트랩이 공격할 수 있는지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshZAxisModify; // 트랩 메쉬 z축 조정

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<TObjectPtr<AActor>> EnemiesInRanged; // 트랩의 공격 범위 안에 들어와있는 적들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackCoolTime; // 트랩의 공격 쿨타임

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeSinceLastAttack; // 트랩의 공격 쿨타임

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage; // 트랩의 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAnimated; // 트랩의 메쉬 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDot; // 도트 공격 트랩 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotDamage; // 도트 공격 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotDuration; // 도트 공격 지속 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotInterval; // 도트 공격 간격

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSlow; // 이동 속도 감소 트랩 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlow"))
	float SlowAmount; // 이동 속도 감소 비율 (0.0 ~ 1.0, 예: 0.5는 50% 감소)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlow"))
	float SlowDuration; // 이동 속도 감소 지속 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType; // 트랩의 데미지 타입

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UParticleSystemComponent>> ParticleEffectComponents;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> AttackCollisionComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> MeshContainerComponent;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UMeshComponent>> MeshComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USkeletalMeshComponent>> SkeletalMeshComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimMontages", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TObjectPtr<UMaterialInterface> PreviewMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TMap<UMeshComponent*,FDDMaterials> OriginalMaterials;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;
};
