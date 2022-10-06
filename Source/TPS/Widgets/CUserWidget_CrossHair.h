
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_CrossHair.generated.h"


UCLASS()
class TPS_API UCUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()

public:
	//비율, 벌어지는 최대값, 벌어지는 속도, 
	void UpdateSpreadRange(float InAmount, float InInterpSpeed, float InMaxRange);

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


private:
	TArray<class UBorder*> Borders;
	TArray<FVector2D> DefaultAlignments;

private:
	enum class EDirection
	{
		Top =0,Bottom,Left,Right,Max,
	};


private:
	float Amount;
	float InterpSpeed;
	float MaxRange;
};
