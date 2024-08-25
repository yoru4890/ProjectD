// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDProjectileData.h"
#include "DDProjectileBase.generated.h"

UCLASS()
class PROJECTD_API ADDProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDProjectileBase();
	virtual ~ADDProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE const FName& GetRowName() const { return RowName; }

public:
	void SetAssetAndManager(const FDDProjectileData& LoadedAsset, class  UDDProjectileManager* InProjectileManager);
	void InitializeProjectile(float InDamageAmount, TSubclassOf<UDamageType> InDamageType, float InProjectileSpeed, float InMaxSpeed, float InMaxLifeTime, bool InbIsExplosive, float InExplosionRadius, int32 InMaxPenetrationCount);

	void SetProjectileActive(bool bIsActive);

protected:
	UFUNCTION()
	virtual void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void SetupCollisionResponses();
	void SetParticeEffects(const FDDProjectileData& LoadedAsset);
	void SetSound(const FDDProjectileData& LoadedAsset);
	void SetMeshs(const FDDProjectileData& LoadedAsset);

	void OnLifeTimeExpired();
	void StopLifeTimeTimer();

	void LaunchProjectile();
	void SetProjectileState(bool bIsActive);

	void Explode();
	void ApplyDamageToActor(AActor* OtherActor);



private:
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lifetime", meta = (AllowPrivateAccess = "true"))
	float MaxLifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosive", meta = (AllowPrivateAccess = "true"))
	bool bIsExplosive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosive", meta = (AllowPrivateAccess = "true"))
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Penetration", meta = (AllowPrivateAccess = "true"))
	int32 MaxPenetrationCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Penetration", meta = (AllowPrivateAccess = "true"))
	int32 CurrentPenetrationCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> TrailNiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> ImpactEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> TrailEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> FlyingSound;

	UPROPERTY()
	TObjectPtr<class UDDProjectileManager> ProjectileManager;

	FTimerHandle LifeSpanTimerHandle;


};
