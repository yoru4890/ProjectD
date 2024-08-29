// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponRifle.h"
#include "Kismet/GameplayStatics.h"
#include "YSY/Collision/CollisionChannel.h"
#include "YSY/Interface/DamageInterface.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"


ADDWeaponRifle::ADDWeaponRifle()
{
	
}

void ADDWeaponRifle::BeginPlay()
{
	CameraManager = UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager;

}


void ADDWeaponRifle::SubSkill()
{
	//Zoom
	
}

void ADDWeaponRifle::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("AttkackkFire"));
	
	//LineTrace
	FHitResult HitResult;

	const FVector StartTrace = CameraManager->GetCameraLocation();
	const FVector EndTrace = StartTrace + CameraManager->GetActorForwardVector() * AttackRange;
	
	bool IsDetect = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, GTCHANNEL_ENEMYDETECT);
	
	FColor DrawColor = IsDetect ? FColor::Green : FColor::Red;

	//DrawDebugLine(GetWorld(), StartTrace, EndTrace, DrawColor, false, 5.0f);
	if (IsDetect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trace : %s"), *(HitResult.GetActor()->GetFName().ToString()));

		IDamageInterface* HitActor = Cast<IDamageInterface>(HitResult.GetActor());

		if (HitActor)
		{
			FDamageEvent DamageEvent;
			float DamageAmount{ 20.0f };
			AController* EventInstigator{};
			auto DDPlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

			HitActor->ApplyDamage(DamageAmount, DamageEvent, EventInstigator, DDPlayerCharacter);

		}
	}

}

void ADDWeaponRifle::EffectAttack()
{
	
}

