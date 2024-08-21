// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Character/DDCharacterControlData.h"

UDDCharacterControlData::UDDCharacterControlData()
{
	TargetArmLength = 400.0f;
	SocketOffset = FVector(0,40.0,100.0);
	MouseSpeed = 0;
	RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	JumpZVelocity = 700.f;
	AirControl = 0.35f;
	MaxWalkSpeed = 147.f;
	MinAnalogWalkSpeed = 20.f;
	BrakingDecelerationWalking = 2000.f;
}
