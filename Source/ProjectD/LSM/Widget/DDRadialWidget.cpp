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
    // ���÷� 8���� ��ư�� ������ 200���� ��ġ
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

            // ��ä�� ����� �̹����� ���� Material ���� �Ǵ� �ε�
            UMaterial* BaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/0000/LSM/Widget/LSM_M_Corn.LSM_M_Corn"));
            if (!BaseMaterial) continue;

            UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
            DynamicMaterial->SetScalarParameterValue(FName("StartAngle"), StartAngle);

            // ��ä�� ��� �̹����� ��ġ�� ��ġ ���
            float AngleInRadians = FMath::DegreesToRadians((StartAngle + EndAngle) / 2.0f);
            float X = Radius * FMath::Cos(AngleInRadians);
            float Y = Radius * FMath::Sin(AngleInRadians);

            // UButton ����
            UButton* NewButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName(*FString::Printf(TEXT("RadialButton%d"), i)));
            CanvasPanel->AddChild(NewButton);

            // UImage ���� �� Button�� �ڽ����� �߰�
            UImage* ButtonImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), FName(*FString::Printf(TEXT("RadialButtonImage%d"), i)));
            ButtonImage->SetBrushFromMaterial(DynamicMaterial);
            NewButton->AddChild(ButtonImage);

            // UButton�� ���� ����
            if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NewButton->Slot))
            {
                CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
                CanvasSlot->SetPosition(FVector2D(X, Y));
                CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
                CanvasSlot->SetSize(FVector2D(100.0f, 100.0f)); // ��ư ũ�� ����
            }


            // ��ư Ŭ�� �̺�Ʈ ó��
            
        }
    }
}

