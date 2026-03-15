// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/BombMainWidget.h"
#include "PlayerController/BombPlayerController.h"
#include "BombPlayerState.h"
#include "GameState/BombGameState.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"




void UBombMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}


void UBombMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ABombPlayerController* PC = GetOwningPlayer<ABombPlayerController>())
	{
		if (PlayerAvatar)
			PlayerAvatar->SetBrushFromTexture(PC->GetSteamAvatarTexture());

		if (PlayerName)
			PlayerName->SetText(FText::FromString(PC->GetUserName()));
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ABombPlayerState* PS = PC->GetPlayerState<ABombPlayerState>())
		{
			MaxHp = PS->GetMaxHp();
			CurrentHp = MaxHp;
		}
	}

	HpFillCurrent = 1.f;
	HpFillTarget = 1.f;

	if (ProgressBar_Hp) ProgressBar_Hp->SetPercent(1.f);
	if (PlayerHp) PlayerHp->SetText(FText::AsNumber(CurrentHp));

	if (RemainCount) RemainCount->SetVisibility(ESlateVisibility::Hidden);
}


void UBombMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ProgressBar_Hp)
	{
		HpFillCurrent = FMath::FInterpTo(HpFillCurrent, HpFillTarget, InDeltaTime, 3.0f);
		if (FMath::IsNearlyEqual(HpFillCurrent, HpFillTarget, 0.001f))
			HpFillCurrent = HpFillTarget;

		ProgressBar_Hp->SetPercent(HpFillCurrent);
	}
}


void UBombMainWidget::UpdateCurrentRound()
{
	if (ABombGameState* GS = GetWorld()->GetGameState<ABombGameState>())
	{
		if (MatchRound)
		{
			MatchRound->SetText(FText::FromString(FString::FromInt(GS->GetMatchRound())));
		}
	}
}

void UBombMainWidget::UpdateHpWidget()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ABombPlayerState* PS = PC->GetPlayerState<ABombPlayerState>())
		{
			CurrentHp = PS->GetHp();
			MaxHp = PS->GetMaxHp();
			if (PlayerHp) 
				PlayerHp->SetText(FText::AsNumber(CurrentHp));

			const float Target = CurrentHp / (MaxHp * 1.f);

			if (ProgressBar_Hp)
			{
				ProgressBar_Hp->SetPercent(Target);
			}
			HpFillTarget = Target;

			// 디버그: 현재 퍼센트 확인
			UE_LOG(LogTemp, Warning, TEXT("UpdateHpWidget: HP=%d/%d, SetPercent=%.3f"),
				CurrentHp, MaxHp, Target);
		}
	}
}


void UBombMainWidget::UpdateCoinWidget()
{
	// 필요 시: PS->GetCoin() 읽어서 전용 텍스트/이미지 업데이트
}

void UBombMainWidget::VisibleRemainingCount(const int32 Count)
{
	RemainCount->SetVisibility(ESlateVisibility::Visible);
	RemainCount->SetText(FText::FromString(FString::FromInt(Count)));
	GetWorld()->GetTimerManager().SetTimer(VisibleCountHandle, this, &UBombMainWidget::HiddenRemainCount, 2.f, false);
}

void UBombMainWidget::HiddenRemainCount()
{
	GetWorld()->GetTimerManager().ClearTimer(VisibleCountHandle);
	RemainCount->SetVisibility(ESlateVisibility::Hidden);
}
