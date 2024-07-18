// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStruct.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMeshType : uint8
{
	StaticMesh,
	SkeletalMesh,
	Unknown
};

USTRUCT(BlueprintType)
struct FBaseStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name; // ������Ʈ�� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID; // ������Ʈ�� ���� �ĺ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMeshType MeshType; // Ʈ���� �޽� Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MeshType == EMeshType::StaticMesh"))
	TArray<TSoftObjectPtr<UStaticMesh>> StaticMeshs; // Ʈ���� ����ƽ �޽�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MeshType == EMeshType::SkeletalMesh"))
	TArray<TSoftObjectPtr<USkeletalMesh>> SkeletalMeshs; // Ʈ���� ���̷��� �޽�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MeshType == EMeshType::SkeletalMesh"))
	TSoftObjectPtr<UAnimBlueprint> AnimBlueprint; // Ʈ���� �ִϸ��̼� �������Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UParticleSystem>> Effects; // Ʈ���� ���� ����Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> FactoryClass; // Ʈ�� ���丮 Ŭ����
};
