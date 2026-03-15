// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DealLayerWidget.generated.h"


class UCanvasPanel;
class UImage;

DECLARE_DELEGATE(FDealFinished);

USTRUCT()
struct FCardTween
{
	GENERATED_BODY()
	UPROPERTY() UImage* Image = nullptr;
	FVector2D From, To;
	float Time = 0.f;
	float Duration = 0.35f;
	float Delay = 0.f;        // 장당 지연(스태거)
	float AngleFrom = -10.f;
	float AngleTo = 0.f;
	FVector2D ScaleFrom = FVector2D(0.8f, 0.8f);
	FVector2D ScaleTo = FVector2D(1.0f, 1.0f);
};

UCLASS()
class SPINNINGBOMB_API UDealLayerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/** 카드 3장을 순차로 날린다. 좌표는 '화면 픽셀' 기준 */
	void PlayDeal3(const FVector2D& DeckPos, const TArray<FVector2D>& Targets, UTexture2D* CardBackTex, FDealFinished OnFinished);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget)) 
	UCanvasPanel* RootCanvas = nullptr;

private:
	TArray<FCardTween> Tweens;
	FDealFinished Finished;
	bool bPlaying = false;

	UImage* SpawnTempCard(UTexture2D* Tex, const FVector2D& At);
	void ApplyTween(FCardTween& T, float Alpha);
	static float EaseOutCubic(float A) { return 1.f - FMath::Pow(1.f - A, 3.f); }
};