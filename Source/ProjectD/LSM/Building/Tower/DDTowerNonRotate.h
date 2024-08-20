// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/Tower/DDTowerBase.h"
#include "DDTowerNonRotate.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDTowerNonRotate : public ADDTowerBase
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
