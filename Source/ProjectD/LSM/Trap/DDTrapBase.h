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

public:
	FORCEINLINE const FName& GetTrapRowName() const { return TrapRowName; }
	//FORCEINLINE const int32 GetTrapBuildCost() const { return TrapBuildCost; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InitFromDataTable(const FName& RowName, const FDDTrapStruct& TrapData);
	void  SetTrapCanAttack(const bool bInCanAttack);
	virtual void SetTrapAssets(FBaseStruct& LoadedAsset) override;
	void SetMaterialToPreview(bool bCanPay);
	void SetMaterialToOriginal();

protected:
	virtual void Attack();

private:
	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TrapDisplayName; // 트랩의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TrapRowName; // 트랩의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAttack; // 트랩이 공격할 수 있는지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<TObjectPtr<AActor>> TrappedEnemies; // 트랩의 공격 범위 안에 들어와있는 적들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapCoolTime; // 트랩의 공격 쿨타임

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeSinceLastAttack; // 트랩의 공격 쿨타임

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapDamage; // 트랩의 데미지

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> TrapDamageType; // 트랩의 데미지 타입

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
};
