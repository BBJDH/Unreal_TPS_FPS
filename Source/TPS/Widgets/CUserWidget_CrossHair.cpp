
#include "CUserWidget_CrossHair.h"
#include "Global.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"	//�������� �����ϱ� ���� ���
#include "Components/Border.h"	//ũ�ν� ��� ������ ������ ������ ����

void UCUserWidget_CrossHair::UpdateSpreadRange(float InRadius, float InMaxRadius)
{
	Radius = InRadius;
	MaxRadius = InMaxRadius;
}

void UCUserWidget_CrossHair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UCanvasPanel* panel = Cast<UCanvasPanel>(WidgetTree->RootWidget);	//������ �ֻ��� Ŭ����
	CheckNull(panel);

	TArray<UWidget*> widgets = panel->GetAllChildren();
	for (UWidget* widget : widgets)
	{
		UBorder* border = Cast<UBorder>(widget);
		if (!!border)
		{
			UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(border);
			//ĵ���� ���� ����


			DefaultAlignments.Add(slot->GetAlignment());
			Borders.Add(border);
		}
	}

	//for (UBorder* border : Borders)
	//	CLog::Log(border);
}

void UCUserWidget_CrossHair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float minimum = 0.0f;
	float maximum = 0.0f;

	for (int32 i = 0; i < (int32)EDirection::Max; i++)
	{
		UBorder* border = Borders[i];
		UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(border);

		switch ((EDirection)i)
		{
		case EDirection::Top:
		{
			minimum = DefaultAlignments[i].Y;
			maximum = DefaultAlignments[i].Y + MaxRadius;
		}
		break;

		case EDirection::Bottom:
		{
			minimum = DefaultAlignments[i].Y;
			maximum = DefaultAlignments[i].Y - MaxRadius;
		}
		break;

		case EDirection::Left:
		{
			minimum = DefaultAlignments[i].X;
			maximum = DefaultAlignments[i].X + MaxRadius;
		}
		break;

		case EDirection::Right:
		{
			minimum = DefaultAlignments[i].X;
			maximum = DefaultAlignments[i].X - MaxRadius;
		}
		break;
		}

		//���°���ŭ Ȯ��
		float value = FMath::Lerp(minimum,maximum, Radius);



		switch ((EDirection)i)
		{
		case EDirection::Top:
		case EDirection::Bottom:
			slot->SetAlignment(FVector2D(DefaultAlignments[i].X, value));
			break;

		case EDirection::Left:
		case EDirection::Right:
			slot->SetAlignment(FVector2D(value, DefaultAlignments[i].Y));
			break;
		}
	}
}