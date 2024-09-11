// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Widget/DDRadialWidget.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UDDRadialWidget::NativeConstruct()
{
    Super::NativeConstruct();
    ButtonCount = 8;
    // 예시로 8개의 버튼을 반지름 200으로 배치
    //CreateRadialMenu(200.0f);
}

void UDDRadialWidget::OnRadialButtonClicked()
{

}

void UDDRadialWidget::CreateRadialMenu(float Radius)
{
    if (UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(GetRootWidget()))
    {
        float AnglePerButton = 360.0f / ButtonCount;

        for (int32 i = 0; i < ButtonCount; ++i)
        {
            float StartAngle = i * AnglePerButton;
            float EndAngle = StartAngle + AnglePerButton;

            // 부채꼴 모양의 이미지를 위한 Material 생성 또는 로드
            UMaterial* BaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/0000/LSM/Widget/LSM_M_Corn.LSM_M_Corn"));
            if (!BaseMaterial) continue;

            UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
            DynamicMaterial->SetScalarParameterValue(FName("StartAngle"), StartAngle);

            // 부채꼴 모양 이미지를 배치할 위치 계산
            float AngleInRadians = FMath::DegreesToRadians((StartAngle + EndAngle) / 2.0f);
            float X = Radius * FMath::Cos(AngleInRadians);
            float Y = Radius * FMath::Sin(AngleInRadians);

            // UButton 생성
            UButton* NewButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName(*FString::Printf(TEXT("RadialButton%d"), i)));
            CanvasPanel->AddChild(NewButton);

            // UImage 생성 및 Button의 자식으로 추가
            UImage* ButtonImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), FName(*FString::Printf(TEXT("RadialButtonImage%d"), i)));
            ButtonImage->SetBrushFromMaterial(DynamicMaterial);
            NewButton->AddChild(ButtonImage);

            // UButton의 슬롯 설정
            if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NewButton->Slot))
            {
                CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
                CanvasSlot->SetPosition(FVector2D(X, Y));
                CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
                CanvasSlot->SetSize(FVector2D(100.0f, 100.0f)); // 버튼 크기 설정
            }


            // 버튼 클릭 이벤트 처리
            
        }
    }
}

