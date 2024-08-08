// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISplineRoute.generated.h"

UCLASS()
class PROJECTD_API AAISplineRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISplineRoute();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FVector GetSplinePointasWorldPosition(int RouteIndex);
	bool IsSplineEnd(int RouteIndex);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comp", meta = (AllowPrivateAccess = "true"))
	class USplineComponent* SplinePath;
};
