
#include "CUserWidget_CrossHair.h"
#include "Global.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"	//슬롯으로 매핑하기 위한 헤더
#include "Components/Border.h"	//크로스 헤어 위젯의 보더를 가져올 예정

void UCUserWidget_CrossHair::UpdateSpreadRange(float InAmount, float InInterpSpeed, float InMaxRange)
{
	Amount = InAmount;
	InterpSpeed = InInterpSpeed;
	MaxRange = InMaxRange;
}

void UCUserWidget_CrossHair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UCanvasPanel* panel = Cast<UCanvasPanel>(WidgetTree->RootWidget);	//위젯의 최상위 클래스
	CheckNull(panel);

	TArray<UWidget*> widgets = panel->GetAllChildren();
	for (UWidget* widget : widgets)
	{
		UBorder* border = Cast<UBorder>(widget);
		if (!!border)
		{
			UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(border);
			//캔버스 슬롯 매핑


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
			maximum = DefaultAlignments[i].Y + MaxRange;
		}
		break;

		case EDirection::Bottom:
		{
			minimum = DefaultAlignments[i].Y;
			maximum = DefaultAlignments[i].Y - MaxRange;
		}
		break;

		case EDirection::Left:
		{
			minimum = DefaultAlignments[i].X;
			maximum = DefaultAlignments[i].X + MaxRange;
		}
		break;

		case EDirection::Right:
		{
			minimum = DefaultAlignments[i].X;
			maximum = DefaultAlignments[i].X - MaxRange;
		}
		break;
		}

		//들어온값만큼 확산
		float value = FMath::Lerp(minimum,maximum,Amount);
		//시간보간
		float spread = FMath::FInterpTo(value,maximum,InDeltaTime,InterpSpeed);


		switch ((EDirection)i)
		{
		case EDirection::Top:
		case EDirection::Bottom:
			slot->SetAlignment(FVector2D(DefaultAlignments[i].X, spread));
			break;

		case EDirection::Left:
		case EDirection::Right:
			slot->SetAlignment(FVector2D(spread, DefaultAlignments[i].Y));
			break;
		}
	}
}