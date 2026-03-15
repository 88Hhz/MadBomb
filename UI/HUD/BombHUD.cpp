// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BombHUD.h"
#include "PlayerController/BombPlayerController.h"

#include "UI/ChatSystem/ChatWidget.h"

#include "UI/InGame/BombMainWidget.h"
#include "UI/InGame/GameStateWidget.h"
#include "UI/InGame/CardWidget.h"
#include "UI/InGame/DealLayerWidget.h"

#include "BombPlayerState.h"



ABombHUD::ABombHUD()
{
	
}


void ABombHUD::BeginPlay()
{
	Super::BeginPlay();

	TryReportClientReady();

	APlayerController* Controller = GetOwningPlayerController();
	if (Controller) 
	{
		if (Controller->IsLocalController())
		{
			if (WBP_MainWidget)
			{
				MainWidget = CreateWidget<UBombMainWidget>(Controller, WBP_MainWidget);
				MainWidget->AddToViewport(1);
			}

			if (WBP_ChatWidget)
			{
				ChatWidget = CreateWidget<UChatWidget>(Controller, WBP_ChatWidget);
				ChatWidget->AddToViewport(2);
			}
		}
	}	


	if (!DealLayer && WBP_DealLayer)
	{
		DealLayer = CreateWidget<UDealLayerWidget>(GetWorld(), WBP_DealLayer);
		if (DealLayer) DealLayer->AddToViewport(9999);
	}

}

void ABombHUD::TryReportClientReady()
{
	if (bAnnouncedReady) return;

	APlayerController* PC = GetOwningPlayerController();
	if (!PC || !PC->IsLocalController())
	{
		// 로컬 클라가 아니면 보고할 필요 없음 (Dedicated 서버 HUD는 없음)
		return;
	}

	// PlayerState가 아직 안 붙었을 수 있으므로 체크
	if (ABombPlayerController* BPC = Cast<ABombPlayerController>(PC))
	{
		if (ABombPlayerState* PS = BPC->GetPlayerState<ABombPlayerState>())
		{
			// 서버에 "나 준비됨" 보고 (서버에서 중복 방어 권장)
			BPC->Server_ReportClientReady();
			bAnnouncedReady = true;
			return;
		}
	}

	// 아직 준비가 덜 됐다면, 다음 틱에 재시도 (짧고 안전)
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ABombHUD::TryReportClientReady);
}

void ABombHUD::ActivateChatUI()
{
	if (ChatWidget)
	{
		ChatWidget->ActivateChatUI();
		UE_LOG(LogTemp, Warning, TEXT("BombHUD: ActivateChatUI()"));
	}
}

void ABombHUD::AddChatMessage(const FString& Message)
{
	if (ChatWidget)
	{
		ChatWidget->AddChatMessage(Message);
	}
}

void ABombHUD::UpdateMainWidget()
{
	UE_LOG(LogTemp, Error, TEXT("BombHUD::UpdateMainWidget() Called."));
	if (MainWidget) 
	{
		MainWidget->UpdateHpWidget();
		MainWidget->UpdateCurrentRound();
		//MainWidget->UpdateCoinWidget();
	}
}


void ABombHUD::PlayDealAndSetCards(const TArray<FName>& DealtCards)
{
	// 연출 준비 안 되어 있으면 바로 표시
	if (!DealLayer || !CardBackTexture || HandTargetPos.Num() < 3)
	{
		SetCardWidgetWithDealtCard(DealtCards);
		return;
	}

	// 연출 후 실제 카드 세팅
	DealLayer->PlayDeal3(
		DeckScreenPos, HandTargetPos, CardBackTexture,
		FDealFinished::CreateLambda([this, DealtCards]()
			{
				SetCardWidgetWithDealtCard(DealtCards);
			})
	);
}


void ABombHUD::SetCardWidgetWithDealtCard(const TArray<FName>& DealtCards)
{
	if (APlayerController* PC = GetOwningPlayerController())
	{
		if (CardWidget == nullptr)
		{
			CardWidget = CreateWidget<UCardWidget>(PC, WBP_CardWidget);
			CardWidget->AddToViewport(3);
		}
		CardWidget->SetWithDealtCards(DealtCards);
	}

}

void ABombHUD::ClearCardWidget()
{
	if (CardWidget != nullptr)
	{
		CardWidget->RemoveFromParent();
		CardWidget = nullptr;
	}
}

void ABombHUD::VisibleRemainingCount(const int32 Count)
{
	UE_LOG(LogTemp, Error, TEXT("BombHUD::VisibleRemainingCount() Called."));
	if (MainWidget)
	{
		MainWidget->VisibleRemainingCount(Count);
	}
}

