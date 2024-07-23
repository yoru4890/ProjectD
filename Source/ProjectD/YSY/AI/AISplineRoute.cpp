// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/AISplineRoute.h"
#include "Components/SplineComponent.h"

AAISplineRoute::AAISplineRoute()
{
	PrimaryActorTick.bCanEverTick = false;

	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));

}

void AAISplineRoute::IncrementRoute()
{
	if (++RouteIndex == SplinePath->GetNumberOfSplinePoints() - 1)
	{
		// TODO : YSY Goal
	}

	UE_LOG(LogTemp, Warning, TEXT("%d"), RouteIndex);
}

FVector AAISplineRoute::GetSplinePointasWorldPosition()
{
	return SplinePath->GetLocationAtSplinePoint(RouteIndex, ESplineCoordinateSpace::World);
}

void AAISplineRoute::BeginPlay()
{
	Super::BeginPlay();
	
}

