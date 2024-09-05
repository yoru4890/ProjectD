// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LSM/Building/DDBuildingBaseData.h"
#include "DDBuildComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartBuild);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PROJECTD_API UDDBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDDBuildComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// ������ ������ �� ȣ��Ǵ� Delegate
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnStartBuild OnStartBuild;

	UFUNCTION(BlueprintCallable)
	void ReadyBuilding(FName RowName);

	UFUNCTION(BlueprintCallable)
	void CancelReadyBuilding();

	UFUNCTION(BlueprintCallable)
	bool PlaceBuilding();

	UFUNCTION(BlueprintCallable)
	void CancelPlacedBuilding();

	// ���ο� �Լ�: Ʈ�� ���׷��̵�
	UFUNCTION(BlueprintCallable)
	bool UpgradeBuilding(const FName& RowName);

	// ���ο� �Լ�: Ʈ�� ���׷��̵�
	UFUNCTION(BlueprintCallable)
	FName GetManagedBuildingRowName();

	UFUNCTION(BlueprintCallable)
	void AllStopTrace();

	UFUNCTION(BlueprintCallable)
	void StopBuildTrace();

	UFUNCTION(BlueprintCallable)
	void StartBuildTrace();

	UFUNCTION(BlueprintCallable)
	void StopManageTrace();

	UFUNCTION(BlueprintCallable)
	void StartManageTrace();

	UFUNCTION()
	void ShowStartBuildWidget();

private:
	UPROPERTY()
	TObjectPtr<class UDDBuildingManager> BuildingManager;

	UPROPERTY()
	TObjectPtr<class ADDGridBuildManager> GridBuildManager;

	UPROPERTY()
	TObjectPtr<class ADDPlayerState> PlayerState;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ADDBuildingBase> PreviewBuilding;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ADDBuildingBase> ManagedBuilding;

	FTimerHandle BuildTraceTimerHandle;

	FTimerHandle ManageTraceTimerHandle;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HitWarningWidgetClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDDCantBuildWidget> HitWarningWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UDDSelectBuildingWidget> SelectBuildingWidgetClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDDSelectBuildingWidget> SelectBuildingWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UDDStartBuildWidget> StartBuildWidgetClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDDStartBuildWidget> StartBuildWidgetInstance;

	bool bIsSetBuilding = false;

	void PerformBuildTrace();

	void PerformManageTrace();

	bool CanPayBuildCost(const FName& RowName) const;
	bool PayBuildCost(const FName& RowName) const;

	void InitWidget();

	void BindEventsToWidget();

	UFUNCTION()
	void ShowSelectBuildingWidget(EBuildingType BuildingType);

	void SetTowerZoneIsHiddenInGame(bool bIsHiddenInGame) const;
	//bool CanPayUpgradeCost(const FName& RowName) const;
	//bool PayUpgradeCost(const FName& RowName) const;

};