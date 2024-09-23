// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponRifle.h"
#include "Kismet/GameplayStatics.h"
#include "YSY/Collision/CollisionChannel.h"
#include "YSY/Interface/DamageInterface.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "YSY/Game/DDPlayerState.h"


ADDWeaponRifle::ADDWeaponRifle()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage>ReloadRef(TEXT("/Game/0000/LJW/Animation/AnimMontage/AM_Reload.AM_Reload"));
	if (nullptr != ReloadRef.Object)
	{
		ReloadAnim = ReloadRef.Object;
	}
}

void ADDWeaponRifle::BeginPlay()
{
	Super::BeginPlay();
	CameraManager = UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager;

	PlayerState = CastChecked<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	LoadedAmmo = 30;
	UnLoadedAmmo = 30;

}


void ADDWeaponRifle::SubSkill()
{
	//Zoom

}

void ADDWeaponRifle::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("AttkackkFire"));

	if (LoadedAmmo < 0)
	{
		return;
	}
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

bool ADDWeaponRifle::Reload()
{
	return AddLoadedRifleAmmo();
}

bool ADDWeaponRifle::AddLoadedRifleAmmo()
{
	if (UnLoadedAmmo <= 0)
	{
		return false;
	}
	int32 AmmoToLoad = FMath::Min(UnLoadedAmmo, MaxAmmoPerMagazine - LoadedAmmo);
	LoadedAmmo += AmmoToLoad;

	OnLoadedAmmoChanged.Broadcast(LoadedAmmo);

	SubtractUnloadedRifleAmmo(AmmoToLoad);

	return true;
}

bool ADDWeaponRifle::SubtractLoadedRifleAmmo(int32 InAmmo)
{
	if (LoadedAmmo >= InAmmo)
	{
		LoadedAmmo -= InAmmo;
		OnLoadedAmmoChanged.Broadcast(LoadedAmmo);
		return true;
	}
	return false;
}

void ADDWeaponRifle::AddUnloadedRifleAmmo(const int32 InAmmo)
{
	if (UnLoadedAmmo == MaxUnLoadedAmmo)
	{
		return;
	}

	if (UnLoadedAmmo + InAmmo > MaxUnLoadedAmmo)
	{
		UnLoadedAmmo = MaxUnLoadedAmmo;
	}
	else
	{
		UnLoadedAmmo += InAmmo;
	}

	OnUnLoadedAmmoChanged.Broadcast(UnLoadedAmmo);
}

bool ADDWeaponRifle::SubtractUnloadedRifleAmmo(const int32 InAmmo)
{
	if (UnLoadedAmmo >= InAmmo)
	{
		UnLoadedAmmo -= InAmmo;
		OnUnLoadedAmmoChanged.Broadcast(UnLoadedAmmo);
		return true;
	}
	return false;
}

void ADDWeaponRifle::EffectAttack()
{

}

