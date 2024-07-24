// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDBuildManager.generated.h"

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
	TArray<FVector> GridCellLocations;
};
