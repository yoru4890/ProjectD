// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapFactory/DDAnimatedTrapFactory.h"
#include "LSM/Trap/DDAnimatedTrap.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/TrapManager/DDTrapAssetManager.h"

ADDTrapBase* UDDAnimatedTrapFactory::CreateTrap(UWorld* World, const FName& TrapName, const FDDTrapStruct& TrapStruct, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	check(World);
	ADDAnimatedTrap* NewTrap = World->SpawnActor<ADDAnimatedTrap>(ADDAnimatedTrap::StaticClass());
	check(NewTrap)

	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);

	UDDTrapAssetManager* TrapAssetManager = MyGameInstance->GetTrapAssetManager();
	check(TrapAssetManager);

	NewTrap->SetActorLocationAndRotation(Location, Rotation);
	NewTrap->SetOwner(Owner);
	NewTrap->SetInstigator(Instigator);
	NewTrap->InitFromDataTable(TrapStruct);

	UStaticMesh* StaticMesh = MyGameInstance->GetTrapAssetManager()->GetStaticMesh(TrapName);
	USkeletalMesh* SkeletalMesh = MyGameInstance->GetTrapAssetManager()->GetSkeletalMesh(TrapName);
	UAnimBlueprint* AnimBlueprint = MyGameInstance->GetTrapAssetManager()->GetAnimBlueprint(TrapName);
	UParticleSystem* ParticleEffect = MyGameInstance->GetTrapAssetManager()->GetParticleEffect(TrapName);
	NewTrap->SetTrapAssets(StaticMesh, SkeletalMesh, AnimBlueprint, ParticleEffect);

	return NewTrap;

	
}
