// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/AISplineRoute.h"
#include "Components/SplineComponent.h"

AAISplineRoute::AAISplineRoute()
{
	PrimaryActorTick.bCanEverTick = false;

	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));

}

FVector AAISplineRoute::GetSplinePointasWorldPosition(int RouteIndex)
{
	return SplinePath->GetLocationAtSplinePoint(RouteIndex, ESplineCoordinateSpace::World);
}

bool AAISplineRoute::IsSplineEnd(int RouteIndex)
{
	return RouteIndex >= SplinePath->GetNumberOfSplinePoints();
}

void AAISplineRoute::BeginPlay()
{
	Super::BeginPlay();
	
}

