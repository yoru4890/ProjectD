// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Tower/DDTowerBase.h"

// Sets default values
ADDTowerBase::ADDTowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ADDTowerBase::~ADDTowerBase()
{
}

// Called when the game starts or when spawned
void ADDTowerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADDTowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

