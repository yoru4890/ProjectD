// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBaseStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name; // ������Ʈ�� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID; // ������Ʈ�� ���� �ĺ���
};
