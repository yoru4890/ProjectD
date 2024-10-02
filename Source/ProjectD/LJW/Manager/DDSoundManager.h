#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DDSoundManager.generated.h"



UCLASS()
class PROJECTD_API UDDSoundManager : public UObject
{
	GENERATED_BODY()
	
public:
	UDDSoundManager();

	void Initialize();

	UFUNCTION(BlueprintCallable, Category = "BGM")
	void PlayBGM(USoundBase* PlayingBGM);

	UFUNCTION(BlueprintCallable, Category = "BGM")
	void StopBGM();
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BGM")
	TObjectPtr<class USoundBase> StartBGM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BGM")
	TObjectPtr<class USoundBase> InGameBGM;

protected:
	UPROPERTY()
	TObjectPtr<class UAudioComponent> AudioComponent;

};
