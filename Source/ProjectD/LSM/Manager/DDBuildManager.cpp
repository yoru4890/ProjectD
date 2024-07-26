// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Manager/DDBuildManager.h"
#include "Components/BoxComponent.h"

// Sets default values
ADDBuildManager::ADDBuildManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);


	GridCellSize = 100.f;
	TrapCellWidth = 3;

}

// Called when the game starts or when spawned
void ADDBuildManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeGridCells();
	DrawDebugBox(GetWorld(), GetActorLocation(), BoxComponent->GetScaledBoxExtent(), FColor::Green, true, -1, 0, 5);

}

// Called every frame
void ADDBuildManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADDBuildManager::InitializeGridCells()
{
	FVector BoxExtent = BoxComponent->GetScaledBoxExtent();
	FVector Origin = GetActorLocation() - FVector(BoxExtent.X, BoxExtent.Y, 0);

	// Calculate the number of cells in X and Y directions
	int32 NumCellsX = FMath::CeilToInt((BoxExtent.X * 2) / GridCellSize);
	int32 NumCellsY = FMath::CeilToInt((BoxExtent.Y * 2) / GridCellSize);

	// Initialize the grid cell locations
	GridCellMap.Empty();

	for (int32 Row = 0; Row < NumCellsY; ++Row)
	{
		for (int32 Column = 0; Column < NumCellsX; ++Column)
		{
			FVector CellLocation = Origin + FVector(Column * GridCellSize + GridCellSize / 2, Row * GridCellSize + GridCellSize / 2, 0);

			// Calculate the height of the cell
			FHitResult HitResult;
			FVector StartLocation = CellLocation + FVector(0, 0, BoxExtent.Z);
			FVector EndLocation = CellLocation - FVector(0, 0, BoxExtent.Z);

			FCollisionQueryParams CollisionParams;
			FGridCell GridCell;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
			{
				CellLocation.Z = HitResult.ImpactPoint.Z;
				GridCell = FGridCell(CellLocation, true);
			}
			else {
				GridCell = FGridCell(CellLocation, false);
			}
			GridCellMap.Add(FIntPoint(Row, Column), GridCell);
		}
	}

	// Debug: Print the grid cell locations
	for (auto& Cell : GridCellMap)
	{
		DrawDebugSphere(GetWorld(), Cell.Value.WorldLocation, 10.0f, 3, FColor::Red, true);
	}
}

const FVector ADDBuildManager::GetNearestGridCellLocation(const FVector& HitLocation) const
{
	FIntPoint GridCellIndex = ConvertWorldLocationToGridCell(HitLocation);

	//UE_LOG(LogTemp, Warning, TEXT("GridCellIndices: X=%d, Y=%d"), GridCellIndex.X, GridCellIndex.Y);

	if (GridCellMap.Contains(GridCellIndex))
	{
		return GridCellMap[GridCellIndex].WorldLocation;
	}

	return FVector(FLT_MAX, FLT_MAX, FLT_MAX);
}

const FIntPoint ADDBuildManager::ConvertWorldLocationToGridCell(const FVector& Location) const
{
	FVector BoxExtent = BoxComponent->GetScaledBoxExtent();
	FVector Origin = GetActorLocation() - FVector(BoxExtent.X - GridCellSize / 2, BoxExtent.Y - GridCellSize / 2, 0);

	FVector LocalPosition = Location - Origin;
	int32 CellX = FMath::RoundToInt(LocalPosition.X / GridCellSize);
	int32 CellY = FMath::RoundToInt(LocalPosition.Y / GridCellSize);

	return FIntPoint(CellY, CellX);
}

const bool ADDBuildManager::CanPlaceTrapAtLocation(const FVector& HitLocation) const
{
	int32 Length = TrapCellWidth / 2;
	FIntPoint Cell = ConvertWorldLocationToGridCell(HitLocation);

	for (int32 Row = Cell.X - Length; Row <= Cell.X + Length; Row++) {
		for (int32 Col = Cell.Y - Length; Col <= Cell.Y + Length; Col++) {
			FIntPoint TempPoint = FIntPoint(Row, Col);
			if (GridCellMap.Contains(TempPoint)) {
				if (!GridCellMap[TempPoint].bCanBuild) {
					return false;
				}
			}
			else {
				return false;
			}
		}
	}

	return true;
}

bool ADDBuildManager::SetGridCellAsOccupied(const FVector& HitLocation)
{
	int32 Length = TrapCellWidth / 2;
	FIntPoint Cell = ConvertWorldLocationToGridCell(HitLocation);

	if (GridCellMap.Contains(Cell)) {
		for (int32 Row = Cell.X - Length; Row <= Cell.X + Length; Row++) {
			for (int32 Col = Cell.Y - Length; Col <= Cell.Y + Length; Col++) {
				FIntPoint TempPoint = FIntPoint(Row, Col);
				if (GridCellMap.Contains(TempPoint)) {
					GridCellMap[TempPoint].bCanBuild = false;
				}
				else {
					return false;
				}
			}
		}

		return true;
	}

	return false;
}

bool ADDBuildManager::SetGridCellAsBlank(const FVector& HitLocation)
{
	int32 Length = TrapCellWidth / 2;
	FIntPoint Cell = ConvertWorldLocationToGridCell(HitLocation);

	if (GridCellMap.Contains(Cell)) {
		for (int32 Row = Cell.X - Length; Row <= Cell.X + Length; Row++) {
			for (int32 Col = Cell.Y - Length; Col <= Cell.Y + Length; Col++) {
				FIntPoint TempPoint = FIntPoint(Row, Col);
				if (GridCellMap.Contains(TempPoint)) {
					GridCellMap[TempPoint].bCanBuild = true;
				}
				else {
					return false;
				}
			}
		}

		return true;
	}

	return false;
}
