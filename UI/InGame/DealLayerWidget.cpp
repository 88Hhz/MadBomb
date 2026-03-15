#include "UI/InGame/DealLayerWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

UImage* UDealLayerWidget::SpawnTempCard(UTexture2D* Tex, const FVector2D& At)
{
	if (!RootCanvas) return nullptr;
	UImage* Img = NewObject<UImage>(this);
	Img->SetBrushFromTexture(Tex, true);
	Img->SetVisibility(ESlateVisibility::HitTestInvisible);
	RootCanvas->AddChild(Img);
	if (auto* CanvasSlot = Cast<UCanvasPanelSlot>(Img->Slot))
	{
		CanvasSlot->SetAutoSize(true);
		CanvasSlot->SetPosition(At);
	}
	return Img;
}

void UDealLayerWidget::PlayDeal3(const FVector2D& DeckPos, const TArray<FVector2D>& Targets,
	UTexture2D* CardBackTex, FDealFinished OnFinished)
{
	Tweens.Reset();
	Finished = OnFinished;
	bPlaying = false;

	if (!RootCanvas || !CardBackTex || Targets.Num() < 3)
	{
		if (Finished.IsBound()) Finished.Execute();
		return;
	}

	const float Stagger = 0.10f;
	for (int32 i = 0; i < 3; ++i)
	{
		if (UImage* Img = SpawnTempCard(CardBackTex, DeckPos))
		{
			FCardTween T;
			T.Image = Img;
			T.From = DeckPos;
			T.To = Targets[i];
			T.Delay = i * Stagger;
			Tweens.Add(T);
		}
	}

	bPlaying = Tweens.Num() > 0;
}

void UDealLayerWidget::ApplyTween(FCardTween& T, float Alpha)
{
	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
	const float E = EaseOutCubic(Alpha);

	if (auto* CanvasSlot = Cast<UCanvasPanelSlot>(T.Image->Slot)) 
	{
		CanvasSlot->SetPosition(FMath::Lerp(T.From, T.To, E));
	}

	T.Image->SetRenderTransformAngle(FMath::Lerp(T.AngleFrom, T.AngleTo, E));
	T.Image->SetRenderScale(FMath::Lerp(T.ScaleFrom, T.ScaleTo, E));
}

void UDealLayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bPlaying) return;

	bool bAllDone = true;
	for (FCardTween& T : Tweens)
	{
		if (T.Delay > 0.f)
		{
			T.Delay -= InDeltaTime;
			if (T.Delay > 0.f) { bAllDone = false; continue; }
			T.Time -= T.Delay; // 넘어간 만큼 보정
		}

		if (T.Time < T.Duration)
		{
			T.Time += InDeltaTime;
			ApplyTween(T, T.Time / T.Duration);
			bAllDone = false;
		}
		else
		{
			ApplyTween(T, 1.f);
		}
	}

	if (bAllDone)
	{
		for (auto& T : Tweens)
			if (T.Image) T.Image->RemoveFromParent();

		Tweens.Reset();
		bPlaying = false;
		if (Finished.IsBound()) Finished.Execute();
	}
}
