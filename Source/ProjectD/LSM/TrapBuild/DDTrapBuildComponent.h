// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DDTrapBuildComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PROJECTD_API UDDTrapBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDDTrapBuildComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	AActor* ReadyTrap(const FName& RowName);

	UFUNCTION(BlueprintCallable)
	void CancleReadyTrap();

	UFUNCTION(BlueprintCallable)
	bool BuildTrap();

	UFUNCTION(BlueprintCallable)
	void CancleBuildTrap();

	// 새로운 함수: 트랩 업그레이드
	UFUNCTION(BlueprintCallable)
	bool UpgradeTrap(const FName& RowName);

	UFUNCTION(BlueprintCallable)
	void AllStopTrace();

	UFUNCTION(BlueprintCallable)
	void StopTrapBuildTrace();

	UFUNCTION(BlueprintCallable)
	void StartTrapBuildTrace();

	UFUNCTION(BlueprintCallable)
	void StopTrapManageTrace();

	UFUNCTION(BlueprintCallable)
	void StartTrapManageTrace();

private:
	UPROPERTY()
	TObjectPtr<class UDDTrapManager> TrapManager;

	UPROPERTY()
	TObjectPtr<class ADDBuildManager> BuildManager;

	UPROPERTY()
	TObjectPtr<class ADDPlayerState> PlayerState;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ADDTrapBase> PreviewTrap;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ADDTrapBase> ManagedTrap;

	FTimerHandle TrapBuildTraceTimerHandle;

	FTimerHandle TrapManageTraceTimerHandle;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HitWarningWidgetClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDDCantBuildWidget> HitWarningWidgetInstance;

	bool bIsSetTrap = false;

	void PerformTrapBuildTrace();

	void PerformTrapManageTrace();

	bool CanPayTrapBuildCost(const FName& RowName) const;
	bool PayTrapBuildCost(const FName& RowName) const;
	bool CanPayTrapUpgradeCost(const FName& RowName) const;
	bool PayTrapUpgradeCost(const FName& RowName) const;

};