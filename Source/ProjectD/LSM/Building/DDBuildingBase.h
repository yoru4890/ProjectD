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
	FORCEINLINE const EBuildingType GetBuildingType() const { return BuildingType;}
	FORCEINLINE const float GetDamage() const { return Damage; }
	FORCEINLINE const TSubclassOf<UDamageType> GetDamageType() const { return DamageType; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InitFromDataTable(const FName& InRowName, const FDDBuildingBaseData& BuildingData);
	virtual void  SetCanAttack(const bool bInCanAttack);
	virtual void SetAssets(FDDBuildingBaseData& LoadedAsset);
	void SetMaterialToPreview(bool bCanPay);
	void SetMaterialToOriginal();

protected:
	virtual void Attack();

	void SetupAttackCollisionResponses();

	UFUNCTION()
	virtual void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void ModifyMeshAndAttackCollision() const;
private:
	void SetParticeEffects(FDDBuildingBaseData& LoadedAsset);
	void SetMeshs(FDDBuildingBaseData& LoadedAsset);


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName; // ������ ���� ǥ�� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName; // ������ ���������̺� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBuildingType BuildingType; // ������ Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CellWidth; // ������ �����ϴ� ���� ũ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridCellSize; // �� ��ĭ�� �����ϴ� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAttack; // ������ ������ �� �ִ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshZAxisModify; // ���� �޽� z�� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<TObjectPtr<AActor>> EnemiesInRanged; // ������ ���� ���� �ȿ� �����ִ� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolTime; // ������ ���� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeSinceLastAttack; // ������ ���� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage; // ������ ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAnimated; // ������ �޽� Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDot; // ��Ʈ ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotDamage; // ��Ʈ ���� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotDuration; // ��Ʈ ���� ���� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotInterval; // ��Ʈ ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSlow; // �̵� �ӵ� ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlow"))
	float SlowAmount; // �̵� �ӵ� ���� ���� (0.0 ~ 1.0, ��: 0.5�� 50% ����)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlow"))
	float SlowDuration; // �̵� �ӵ� ���� ���� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType; // ������ ������ Ÿ��

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UFXSystemAsset> AttackEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UFXSystemAsset> HitEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UShapeComponent> AttackCollisionComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> MeshContainerComponent;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UMeshComponent>> MeshComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USkeletalMeshComponent>> SkeletalMeshComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UStaticMeshComponent>> StaticMeshComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackMontages", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TObjectPtr<UMaterialInterface> PreviewMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TMap<UMeshComponent*,FDDMaterials> OriginalMaterials;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;
};
