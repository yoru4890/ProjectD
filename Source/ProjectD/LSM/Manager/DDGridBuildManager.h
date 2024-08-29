// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDGridBuildManager.generated.h"

USTRUCT(BlueprintType)
struct FGridCell
{
	GENERATED_BODY()

public:
	FVector WorldLocation;
	FVector NormalVector;
	bool bCanBuild;
	bool bIsTowerArea;

	FGridCell()
		: WorldLocation(0)
		, NormalVector(0,0,1)
		, bCanBuild(false)
		, bIsTowerArea(false)
	{}

	// 매개변수를 가진 생성자
	FGridCell(FVector InWorldLocation, FVector InNormalVector, bool bInCanBuild)
		: WorldLocation(InWorldLocation)
		, NormalVector(InNormalVector)
		, bCanBuild(bInCanBuild)
		, bIsTowerArea(false)
	{}

};
UCLASS()
class PROJECTD_API ADDGridBuildManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADDGridBuildManager();

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

	UPROPERTY()
	// Array to store grid cell locations
	TMap<FIntPoint, FGridCell> GridCellMap;

	UPROPERTY(EditAnyWhere)
	TArray<TObjectPtr<UStaticMeshComponent>> TowerZones;

	UPROPERTY(EditAnywhere)
	float PlaneTolerance = 5.f;

	UPROPERTY()
	TObjectPtr<UStaticMesh> TowerZoneMeshAsset;
public:
	FORCEINLINE const float GetGridCellSize() const { return GridCellSize; };
	const FVector GetNearestGridCellLocation(const FVector& HitLocation) const;
	const bool CanPlaceBuildingAtLocation(const FVector& HitLocation, const int32 OccupiedCellWidth, const bool bIsTower) const;
	const bool CanPlaceTowerAtLocation(const FVector& HitLocation, const int32 OccupiedCellWidth) const;
	const FVector GetGridCellNormalVector(const FVector& HitLocation) const;
	bool SetGridCellAsOccupied(const FVector& HitLocation, const int32 OccupiedCellWidth);
	bool SetGridCellAsBlank(const FVector& HitLocation, const int32 OccupiedCellWidth);
	void SetTowerBuildingZoneMaterial(bool bIsHiddenInGame);

private:
	const FIntPoint ConvertWorldLocationToGridCell(const FVector& Location) const;
	const bool IsPointOnSamePlane(const FVector& InPointWorldLocation, const FVector& StandardPointWorldLocation, const FVector& PlaneNormalVector) const;

	UFUNCTION(CallInEditor, Category = "Init")
	void AddTowerZone();

	UFUNCTION(CallInEditor, Category = "Debug")
	void ShowDeugGrid();

	void UpdateTowerZone();


	
};
