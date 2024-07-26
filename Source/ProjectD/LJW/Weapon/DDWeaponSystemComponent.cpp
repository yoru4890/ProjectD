// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponSystemComponent.h"
#include "YSY/Game/DDGameSingleton.h"
#include "LJW/Weapon/DDWeaponRifle.h"

// Sets default values for this component's properties
UDDWeaponSystemComponent::UDDWeaponSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDDWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UDDWeaponSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDDWeaponSystemComponent::InitializeWeapon()
{
	// �̱��濡�� data���� ����
	FDDWeaponData MondungiData;
	TArray<FName> WeaponNames;

	Weapon1->InitData(MondungiData);

	auto TempWeapon = NewObject<ADDWeaponRifle>();
	Weapons.Add(TempWeapon);
	//Weapon[0].Attack();

	/*Weapons[0]->InitData("������");
	Weapons[1]->InitData("�����̤�������");
	Weapons[2]->InitData("���դ���");
	Weapons[3]->InitData("��������");
	Weapons[4]->InitData("������");*/

	//// TODO : LJW MyInit()
	//Weapons.SetNum(static_cast<int32>(EWeaponType::Unknown));

	//FDDWeaponData TempData;
	//TArray<FName> WeaponNames;
	//WeaponNames.Add("������");
	//for (int i{}; i < Weapons.Num(); i++)
	//{
	//	Weapons[i].InitData(WeaponDatas[i]);
	//}
	//initData("TrapName");
	//auto WeaponDatas = UDDGameSingleton::Get().GetEnemyDataTable();
}

