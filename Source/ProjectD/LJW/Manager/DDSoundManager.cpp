

#include "LJW/Manager/DDSoundManager.h"

UDDSoundManager::UDDSoundManager()
{
	
	//Start BGM
	static ConstructorHelpers::FObjectFinder<USoundBase> StartRef(TEXT("/Script/Engine.SoundWave'/Game/0000/LJW/Sounds/BGM/WAV_Colossal_Hybrid_Action_Climax_Full.WAV_Colossal_Hybrid_Action_Climax_Full'"));
	if (StartRef.Succeeded())
	{
		StartBGM = StartRef.Object;
	}
	/*
	//InGame BGM
	static ConstructorHelpers::FObjectFinder<USoundBase> InGameRef(TEXT(""));
	if (InGameRef.Succeeded())
	{
		InGameBGM = InGameRef.Object;
	}
	*/
	
}

void UDDSoundManager::Initialize()
{
	//AudioComponent 초기화
	if (!AudioComponent)
	{
		//AudioComponent = NewObject<UAudioComponent>(this);
		//AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), StartBGM, 1.f, 1.f, 0.f, nullptr, true, true);
		
		if (AudioComponent)
		{
			
			//AudioComponent->RegisterComponent();
			UE_LOG(LogTemp, Warning, TEXT("DDSound : Has AudioComponent"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DDSound : No AudioComponent Data"));
		}

	}
	
}

void UDDSoundManager::PlayBGM(USoundBase* PlayingBGM)
{
	if (AudioComponent == nullptr)
	{
		AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), PlayingBGM, 1.0f, 1.0f, 0, nullptr, true, false);
		AudioComponent->bAutoActivate = false; //자동 재생 비활성화
	}
	else
	{
		if (AudioComponent && PlayingBGM)
		{
			//BGM 이 재생 중이면 무시
			if (AudioComponent->IsPlaying())
			{
				return;
			}

			//SoundCue 설정 및 재생
			if (PlayingBGM->IsLooping())
			{
				UE_LOG(LogTemp, Warning, TEXT("IsLooping"));
			}
			AudioComponent->SetSound(PlayingBGM);
			AudioComponent->Play();
		}
	}
}

void UDDSoundManager::StopBGM()
{
	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();
		
	}
}

void UDDSoundManager::FadeOutBGM()
{
	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->FadeOut(3.0f, 0.0f);

	}
	
}
