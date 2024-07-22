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
    UE_LOG(LogTemp, Warning, TEXT("Box Extent: %s"), *BoxExtent.ToString());
    FVector Origin = GetActorLocation() - BoxExtent;

    // Calculate the number of cells in X and Y directions
    int32 NumCellsX = FMath::CeilToInt((BoxExtent.X * 2) / GridCellSize);
    int32 NumCellsY = FMath::CeilToInt((BoxExtent.Y * 2) / GridCellSize);

    // Initialize the grid cell locations
    GridCellLocations.Empty();

    for (int32 Row = 0; Row < NumCellsY; ++Row)
    {
        for (int32 Column = 0; Column < NumCellsX; ++Column)
        {
            FVector CellLocation = Origin - BoxExtent + FVector(Column * GridCellSize + GridCellSize / 2, Row * GridCellSize + GridCellSize / 2, 0);
            GridCellLocations.Add(CellLocation);
        }
    }

    // Debug: Print the grid cell locations
    for (const FVector& Location : GridCellLocations)
    {
        DrawDebugSphere(GetWorld(), Location, 10.0f, 3, FColor::Red, true);
    }
}

