// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponCudgel.h"
#include "GameFramework/Character.h"
#include "YSY/Collision/CollisionChannel.h"
#include "YSY/Interface/DamageInterface.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

ADDWeaponCudgel::ADDWeaponCudgel()
{
	//�ݸ��� ������Ʈ ����
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

	IDamageInterface* HitActor = Cast<IDamageInterface>(OtherActor);

	FDamageEvent DamageEvent;
	float DamageAmount{ 20.0f };
	AController* EventInstigator{};

	HitActor->ApplyDamage(DamageAmount, DamageEvent, EventInstigator, this);
}



