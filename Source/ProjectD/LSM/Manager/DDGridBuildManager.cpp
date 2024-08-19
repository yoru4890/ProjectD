// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Manager/DDGridBuildManager.h"
#include "Components/BoxComponent.h"
#include "YSY/Collision/CollisionChannel.h"
// Sets default values
ADDGridBuildManager::ADDGridBuildManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);


	GridCellSize = 100.f;

}

// Called when the game starts or when spawned
void ADDGridBuildManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeGridCells();
	UpdateTowerZone();
	DrawDebugBox(GetWorld(), GetActorLocation(), BoxComponent->GetScaledBoxExtent(), FColor::Green, true, -1, 0, 5);

}

// Called every frame
void ADDGridBuildManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADDGridBuildManager::InitializeGridCells()
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
				GridCell = FGridCell(CellLocation, HitResult.ImpactNormal, true);
			}
			else {
				CellLocation.Z = -10000.f;
				GridCell = FGridCell(CellLocation, HitResult.ImpactNormal, false);
			}
			GridCellMap.Add(FIntPoint(Row, Column), GridCell);
		}
	}
}

const FVector ADDGridBuildManager::GetNearestGridCellLocation(const FVector& HitLocation) const
{
	FIntPoint GridCellIndex = ConvertWorldLocationToGridCell(HitLocation);

	if (GridCellMap.Contains(GridCellIndex))
	{
		return GridCellMap[GridCellIndex].WorldLocation;
	}

	return FVector(FLT_MAX, FLT_MAX, FLT_MAX);
}

const FIntPoint ADDGridBuildManager::ConvertWorldLocationToGridCell(const FVector& Location) const
{
	FVector BoxExtent = BoxComponent->GetScaledBoxExtent();
	FVector Origin = GetActorLocation() - FVector(BoxExtent.X - GridCellSize / 2, BoxExtent.Y - GridCellSize / 2, 0);

	FVector LocalPosition = Location - Origin;
	int32 CellX = FMath::RoundToInt(LocalPosition.X / GridCellSize);
	int32 CellY = FMath::RoundToInt(LocalPosition.Y / GridCellSize);

	return FIntPoint(CellY, CellX);
}

const bool ADDGridBuildManager::CanPlaceBuildingAtLocation(const FVector& HitLocation, const int32 OccupiedCellWidth, const bool bIsTower) const
{
	int32 Length = OccupiedCellWidth / 2;
	FIntPoint Cell = ConvertWorldLocationToGridCell(HitLocation);
	if (!GridCellMap.Contains(Cell)) {
		return false;
	}

	FVector InNormalVector = GridCellMap[Cell].NormalVector;
	FVector StandardPoint = GridCellMap[Cell].WorldLocation;

	for (int32 Row = Cell.X - Length; Row <= Cell.X + Length; Row++) {
		for (int32 Col = Cell.Y - Length; Col <= Cell.Y + Length; Col++) {
			FIntPoint TempPoint = FIntPoint(Row, Col);
			if (GridCellMap.Contains(TempPoint)) {
				if (!GridCellMap[TempPoint].bCanBuild) {
					return false;
				}
				FVector PointLocation = GridCellMap[TempPoint].WorldLocation;
				// 점이 평면 위에 있는지 확인
				if (!IsPointOnSamePlane(PointLocation, StandardPoint, InNormalVector)) {
					return false;
				}

				if (GridCellMap[TempPoint].bIsTowerArea != bIsTower)
				{
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

const bool ADDGridBuildManager::CanPlaceTowerAtLocation(const FVector& HitLocation, const int32 OccupiedCellWidth) const
{
	int32 Length = OccupiedCellWidth / 2;
	FIntPoint Cell = ConvertWorldLocationToGridCell(HitLocation);
	if (!GridCellMap.Contains(Cell)) {
		return false;
	}

	FVector InNormalVector = GridCellMap[Cell].NormalVector;
	FVector StandardPoint = GridCellMap[Cell].WorldLocation;

	return false;
}

const FVector ADDGridBuildManager::GetGridCellNormalVector(const FVector& HitLocation) const
{
	FIntPoint Cell = ConvertWorldLocationToGridCell(HitLocation);
	if (GridCellMap.Contains(Cell)) {
		FVector outNormalVector = GridCellMap[Cell].NormalVector;
		return outNormalVector;
	}
	return FVector(FLT_MAX, FLT_MAX, FLT_MAX);
}

bool ADDGridBuildManager::SetGridCellAsOccupied(const FVector& HitLocation, const int32 OccupiedCellWidth)
{
	int32 Length = OccupiedCellWidth / 2;
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

bool ADDGridBuildManager::SetGridCellAsBlank(const FVector& HitLocation, const int32 OccupiedCellWidth)
{
	int32 Length = OccupiedCellWidth / 2;
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

const bool ADDGridBuildManager::IsPointOnSamePlane(const FVector& InPointWorldLocation, const FVector& StandardPointWorldLocation, const FVector& PlaneNormalVector) const
{
	// 평면 위에 있는 점이라면 내적했을 때, 값이 0
	float DistanceToPlane = FVector::DotProduct(PlaneNormalVector, InPointWorldLocation - StandardPointWorldLocation);
	return FMath::Abs(DistanceToPlane) <= KINDA_SMALL_NUMBER;
}

void ADDGridBuildManager::AddTowerZone()
{
	UBoxComponent* NewTowerZone = NewObject<UBoxComponent>(this);
	// 이름 설정
	FName TowerZoneName = FName(*FString::Printf(TEXT("%s:TowerZone"), *NewTowerZone->GetFName().ToString()));
	NewTowerZone->Rename(*TowerZoneName.ToString());

	NewTowerZone->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));
	NewTowerZone->SetupAttachment(RootComponent);
	NewTowerZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	NewTowerZone->SetCollisionResponseToChannel(GTCHANNEL_BUILDINGTRACE, ECR_Block);


	NewTowerZone->RegisterComponent();
	TowerZones.Add(NewTowerZone);
}

void ADDGridBuildManager::ShowDeugGrid()
{
	// Debug: Print the grid cell locations
	for (auto& Cell : GridCellMap)
	{
		if (Cell.Value.bCanBuild)
		{
			if (Cell.Value.bIsTowerArea)
			{
				DrawDebugSphere(GetWorld(), Cell.Value.WorldLocation, 10.0f, 3, FColor::Blue, false,5.0f);
			}
			else
			{
				DrawDebugSphere(GetWorld(), Cell.Value.WorldLocation, 10.0f, 3, FColor::Green, false, 5.0f);
			}
		}
		else
		{
			DrawDebugSphere(GetWorld(), Cell.Value.WorldLocation, 10.0f, 3, FColor::Red, false, 5.0f);
		}
	}
}

void ADDGridBuildManager::UpdateTowerZone()
{
	for (UBoxComponent* TowerZone : TowerZones)
	{
		FVector ZoneExtent = TowerZone->GetScaledBoxExtent();
		FVector ZoneOrigin = TowerZone->GetComponentLocation() - FVector(ZoneExtent.X, ZoneExtent.Y, 0);

		int32 NumCellsX = FMath::CeilToInt((ZoneExtent.X * 2) / GridCellSize);
		int32 NumCellsY = FMath::CeilToInt((ZoneExtent.Y * 2) / GridCellSize);

		for (int32 Row = 0; Row < NumCellsY; ++Row)
		{
			for (int32 Column = 0; Column < NumCellsX; ++Column)
			{
				FVector CellLocation = ZoneOrigin + FVector(Column * GridCellSize + GridCellSize / 2, Row * GridCellSize + GridCellSize / 2, 0);
				FIntPoint CellIndex = ConvertWorldLocationToGridCell(CellLocation);

				if (GridCellMap.Contains(CellIndex))
				{
					GridCellMap[CellIndex].bIsTowerArea = true;
				}
			}
		}
	}
}
