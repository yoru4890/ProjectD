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
		// TODO : Goal
	}
}

FVector AAISplineRoute::GetSplinePointasWorldPosition()
{
	return SplinePath->GetLocationAtSplinePoint(RouteIndex, ESplineCoordinateSpace::World);
}

void AAISplineRoute::BeginPlay()
{
	Super::BeginPlay();
	
}

