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
	bool bCanBuild;

	FGridCell()
		: WorldLocation(0)
		, bCanBuild(false)
	{}

	// 매개변수를 가진 생성자
	FGridCell(FVector InWorldLocation, bool bInCanBuild)
		: WorldLocation(InWorldLocation)
		, bCanBuild(bInCanBuild)
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

	UPROPERTY(EditAnywhere, Category = "Init")
	int32 TrapCellWidth;

	// Box collision component to define the bounds of the BuildManager
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<class UBoxComponent> BoxComponent;

	// Function to initialize grid cells
	void InitializeGridCells();

	UPROPERTY(EditAnywhere, Category = "Init")
	// Array to store grid cell locations
	TMap<FIntPoint, FGridCell> GridCellMap;

public:
	const FVector GetNearestGridCellLocation(const FVector& HitLocation) const;
	const bool CanPlaceTrapAtLocation(const FVector& HitLocation) const;
	bool SetGridCellAsOccupied(const FVector& HitLocation);
	bool SetGridCellAsBlank(const FVector& HitLocation);

private:
	const FIntPoint ConvertWorldLocationToGridCell(const FVector& Location) const;
	
};
