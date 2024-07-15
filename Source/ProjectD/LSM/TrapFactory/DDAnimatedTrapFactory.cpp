// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapFactory/DDAnimatedTrapFactory.h"
#include "LSM/Trap/DDAnimatedTrap.h"

AActor* UDDAnimatedTrapFactory::CreateTrap(UWorld* World, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	check(World);
	FActorSpawnParameters SpawnParams;
	ADDAnimatedTrap* NewTrap = World->SpawnActor<ADDAnimatedTrap>(ADDAnimatedTrap::StaticClass(), Location, Rotation);
	if (NewTrap)
	{
		NewTrap->SetOwner(Owner);
		//NewTrap->Instigator = Instigator;
		// Initialize NewTrap as needed
	}
	return NewTrap;

}
