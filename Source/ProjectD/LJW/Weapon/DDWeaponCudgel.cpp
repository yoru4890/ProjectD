// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponCudgel.h"
#include "GameFramework/Character.h"

ADDWeaponCudgel::ADDWeaponCudgel()
{
	//콜리전 컴포넌트 생성
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	
}

void ADDWeaponCudgel::SubSkill()
{
	//Cudgel Skill
	
}

