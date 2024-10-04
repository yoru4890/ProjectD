// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponCudgel.h"
#include "GameFramework/Character.h"
#include "YSY/Collision/CollisionChannel.h"
#include "YSY/Interface/DamageInterface.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ADDWeaponCudgel::ADDWeaponCudgel()
{
	//콜리전 컴포넌트 생성
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CollisionCapsule->InitCapsuleSize(10.0f, 50.0f);
	CollisionCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionCapsule->SetCollisionResponseToChannel(GTCHANNEL_ENEMY, ECollisionResponse::ECR_Overlap);
	CollisionCapsule->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	CollisionCapsule->SetupAttachment(RootComponent);
	CollisionCapsule->AddToRoot();
	//Begin Overlap
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ADDWeaponCudgel::OnOverlapBegin);

	//Collision Disabled
	CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADDWeaponCudgel::SubSkill()
{
	//Cudgel Skill
	
}

void ADDWeaponCudgel::Attack()
{
	Super::Attack();
	//Cudgel Attack
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
}

void ADDWeaponCudgel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	CollisionCapsule->RemoveFromRoot();
}

void ADDWeaponCudgel::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UE_LOG(LogTemp, Warning, TEXT("Overlap : %s"), *(OtherActor->GetFName().ToString()));

	IDamageInterface* HitActor = Cast<IDamageInterface>(OtherActor);
	if (HitActor)
	{
		FDamageEvent DamageEvent;
		float DamageAmount{ 20.0f };
		AController* EventInstigator{};
		auto DDPlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		HitActor->ApplyDamage(DamageAmount, DamageEvent, EventInstigator, DDPlayerCharacter);
		// 여기서 타격음
	}
}



