// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDBuildManager.generated.h"

USTRUCT(BlueprintType)
struct FGridCell
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FVector WorldLocation;
	UPROPERTY(VisibleAnywhere)
	FVector NormalVector;
	UPROPERTY(VisibleAnywhere)
	bool bCanBuild;
	UPROPERTY(VisibleAnywhere)
	bool bIsTowerArea;

	FGridCell()
		: WorldLocation(0)
		, NormalVector(0,0,1)
		, bCanBuild(false)
		, bIsTowerArea(false)
	{}

	// �Ű������� ���� ������
	FGridCell(FVector InWorldLocation, FVector InNormalVector, bool bInCanBuild)
		: WorldLocation(InWorldLocation)
		, NormalVector(InNormalVector)
		, bCanBuild(bInCanBuild)
		, bIsTowerArea(false)
	{}

};

UCLASS()
class PROJECTD_API ADDBuildManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADDBuildManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditAnywhere, Category = "Init")
	float GridCellSize;


	// Box collision component to define the bounds of the BuildManager
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<class UBoxComponent> BoxComponent;

	// Function to initialize grid cells
	void InitializeGridCells();

	UPROPERTY(EditAnywhere, Category = "Init")
	// Array to store grid cell locations
	TMap<FIntPoint, FGridCell> GridCellMap;

	UPROPERTY(EditAnywhere, Category = "Init")
	TArray<TObjectPtr<UBoxComponent>> TowerZones;

public:
	FORCEINLINE const float GetGridCellSize() const { return GridCellSize; };
	const FVector GetNearestGridCellLocation(const FVector& HitLocation) const;
	const bool CanPlaceTrapAtLocation(const FVector& HitLocation, const int32 OccupiedCellWidth) const;
	const bool CanPlaceTowerAtLocation(const FVector& HitLocation, const int32 OccupiedCellWidth) const;
	const FVector GetGridCellNormalVector(const FVector& HitLocation) const;
	bool SetGridCellAsOccupied(const FVector& HitLocation, const int32 OccupiedCellWidth);
	bool SetGridCellAsBlank(const FVector& HitLocation, const int32 OccupiedCellWidth);

private:
	const FIntPoint ConvertWorldLocationToGridCell(const FVector& Location) const;
	const bool IsPointOnSamePlane(const FVector& InPointWorldLocation, const FVector& StandardPointWorldLocation, const FVector& PlaneNormalVector) const;

	UFUNCTION(CallInEditor, Category = "Init")
	void AddTowerZone();

	void UpdateTowerZone();
	
};
