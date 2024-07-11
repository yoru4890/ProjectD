// Fill out your copyright notice in the Description page of Project Settings.

#include "LSM/Trap/DDTrapBase.h"

// Sets default values
ADDTrapBase::ADDTrapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//RootComponent = Mesh;

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> TrapMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	//if (TrapMeshRef.Object) {
	//	Mesh->SetStaticMesh(TrapMeshRef.Object);
	//}

}

// Called when the game starts or when spawned
void ADDTrapBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADDTrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

