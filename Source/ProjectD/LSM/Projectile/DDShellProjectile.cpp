// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Projectile/DDShellProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ADDShellProjectile::ADDShellProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/0000/LSM/Projectile/tank_shell_bullet.tank_shell_bullet"));

	if (MeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	}
	StaticMeshComponent->SetWorldScale3D(FVector(0.3f));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);

}

// Called when the game starts or when spawned
void ADDShellProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADDShellProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

