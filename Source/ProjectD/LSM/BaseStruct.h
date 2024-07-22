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
	FName Name; // 오브젝트의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID; // 오브젝트의 고유 식별자

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMeshType MeshType; // 트랩의 메쉬 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MeshType == EMeshType::StaticMesh"))
	TArray<TSoftObjectPtr<UStaticMesh>> StaticMeshs; // 스태틱 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MeshType == EMeshType::SkeletalMesh"))
	TArray<TSoftObjectPtr<USkeletalMesh>> SkeletalMeshs; // 스켈레톤 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MeshType == EMeshType::SkeletalMesh"))
	TSoftObjectPtr<UAnimBlueprint> AnimBlueprint; // 애니메이션 블루프린트

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UParticleSystem>> Effects; // 공격 이펙트

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> FactoryClass; // 팩토리 클래스
};
