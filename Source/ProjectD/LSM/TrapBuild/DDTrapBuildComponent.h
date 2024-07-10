// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DDTrapBuildComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTD_API UDDTrapBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDDTrapBuildComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ReadyTrap(class ADDTrapBase* Trap) const;

	UFUNCTION(BlueprintCallable)
	void CancleReadyTrap(class ADDTrapBase* Trap) const;

	UFUNCTION(BlueprintCallable)
	void BuildTrap(class ADDTrapBase* Trap) const;

	UFUNCTION(BlueprintCallable)
	void CancleBuildTrap(class ADDTrapBase* Trap) const;

	// 새로운 함수: 트랩 업그레이드
	UFUNCTION(BlueprintCallable)
	void UpgradeTrap(class ADDTrapBase* Trap) const;

};