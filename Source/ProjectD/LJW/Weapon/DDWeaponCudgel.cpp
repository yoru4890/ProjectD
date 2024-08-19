// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponCudgel.h"
#include "GameFramework/Character.h"
#include "YSY/Collision/CollisionChannel.h"

ADDWeaponCudgel::ADDWeaponCudgel()
{
	//콜리전 컴포넌트 생성
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CollisionCapsule->InitCapsuleSize(10.0f, 50.0f);
	CollisionCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionCapsule->SetCollisionResponseToChannel(GTCHANNEL_ENEMY, ECollisionResponse::ECR_Overlap);
	CollisionCapsule->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	CollisionCapsule->SetupAttachment(RootComponent);
	//Begin Overlap
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ADDWeaponCudgel::OnOverlapBegin);
}

void ADDWeaponCudgel::SubSkill()
{
	//Cudgel Skill
	
}

void ADDWeaponCudgel::Attack()
{
	//Cudgel Attack
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
}

void ADDWeaponCudgel::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UE_LOG(LogTemp, Warning, TEXT("Overlap : %s"), *(OtherActor->GetFName().ToString()));

}



