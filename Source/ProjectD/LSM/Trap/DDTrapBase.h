// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDTrapBase.generated.h"

UCLASS()
class PROJECTD_API ADDTrapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDTrapBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Trap)
	TObjectPtr<class UStaticMeshComponent> Mesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
