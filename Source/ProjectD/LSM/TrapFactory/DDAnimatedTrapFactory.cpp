// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapFactory/DDAnimatedTrapFactory.h"
#include "LSM/Trap/DDAnimatedTrap.h"

AActor* UDDAnimatedTrapFactory::CreateTrap(UWorld* World, const FName& TrapName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	check(World);
	ADDAnimatedTrap* NewTrap = World->SpawnActor<ADDAnimatedTrap>(ADDAnimatedTrap::StaticClass(), Location, Rotation);
	if (NewTrap)
	{
		NewTrap->SetActorLocationAndRotation(Location, Rotation);
		NewTrap->SetOwner(Owner);
		NewTrap->SetInstigator(Instigator);
	}
	return NewTrap;

}
