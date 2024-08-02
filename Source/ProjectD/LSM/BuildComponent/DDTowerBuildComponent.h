// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DDTowerBuildComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTD_API UDDTowerBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDDTowerBuildComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	AActor* ReadyTower(const FName& RowName);

	UFUNCTION(BlueprintCallable)
	void CancleReadyTower();

	UFUNCTION(BlueprintCallable)
	bool BuildTower();

	UFUNCTION(BlueprintCallable)
	void CancleBuildTower();

	// 새로운 함수: 트랩 업그레이드
	UFUNCTION(BlueprintCallable)
	bool UpgradeTower(const FName& RowName);

	UFUNCTION(BlueprintCallable)
	void AllStopTrace();

	UFUNCTION(BlueprintCallable)
	void StopTowerBuildTrace();

	UFUNCTION(BlueprintCallable)
	void StartTowerBuildTrace();

	UFUNCTION(BlueprintCallable)
	void StopTowerManageTrace();

	UFUNCTION(BlueprintCallable)
	void StartTowerManageTrace();

private:
	UPROPERTY()
	TObjectPtr<class UDDTowerManager> TowerManager;

	UPROPERTY()
	TObjectPtr<class ADDBuildManager> BuildManager;

	UPROPERTY()
	TObjectPtr<class ADDPlayerState> PlayerState;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ADDTowerBase> PreviewTower;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ADDTowerBase> ManagedTower;

	FTimerHandle TowerBuildTraceTimerHandle;

	FTimerHandle TowerManageTraceTimerHandle;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HitWarningWidgetClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDDCantBuildWidget> HitWarningWidgetInstance;

	bool bIsSetTower = false;

	void PerformTowerBuildTrace();

	void PerformTowerManageTrace();

	bool CanPayTowerBuildCost(const FName& RowName) const;
	bool PayTowerBuildCost(const FName& RowName) const;
	bool CanPayTowerUpgradeCost(const FName& RowName) const;
	bool PayTowerUpgradeCost(const FName& RowName) const;

		
};
