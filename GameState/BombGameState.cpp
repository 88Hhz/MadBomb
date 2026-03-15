// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BombGameState.h"
#include "GameMode/BombGameMode.h"
#include "BombPlayerState.h"
#include "UI/HUD/BombHUD.h"


#include "Net/UnrealNetwork.h"	//DOREPLIFETIME
#include "Kismet/GameplayStatics.h"


ABombGameState::ABombGameState()
{
	GameModeClass = ABombGameMode::StaticClass();
	AuthorityGameMode = GameModeClass.GetDefaultObject();

	CardFX = CreateDefaultSubobject<UBombCardManagerComponent>(TEXT("CardManager"));
}

void ABombGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABombGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABombGameState, NumOfPlayerInGame);

	DOREPLIFETIME(ABombGameState, CurrentTurnPlayerOrder);
	DOREPLIFETIME(ABombGameState, MatchRound);
}

void ABombGameState::SetCurrentTurnOfPlayer(const int32 NewOrder)
{
	if (!HasAuthority()) return;

	CurrentTurnPlayerOrder = NewOrder;
	HandleCurrentTurnChanged();
}

void ABombGameState::SetMatchRound(const int32 NewRound)
{
	if (!HasAuthority()) return;

	MatchRound = NewRound;

	HandleMatchRoundChanged();
}

void ABombGameState::SetSeatTarget(int32 SeatIdx, AActor* TargetPoint)
{
	if (!CardFX) return;
	if (!TargetPoint) return;

	FSeatTarget Data;
	Data.TargetPoint = TargetPoint;
	CardFX->SeatTargets.Add(SeatIdx, Data);
}

void ABombGameState::SetCenterPoint(AActor* Center)
{
	if (!CardFX) return;
	CardFX->CenterPoint = Center;
}

void ABombGameState::OnRep_MatchRound()
{
	HandleMatchRoundChanged();
}

void ABombGameState::OnRep_CurrentTurnPlayerOrder()
{
	HandleCurrentTurnChanged();
}

void ABombGameState::HandleMatchRoundChanged()
{
	if (UWorld* W = GetWorld())
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(W, 0))
		{
			if (ABombHUD* HUD = PC->GetHUD<ABombHUD>())
			{
				HUD->UpdateMainWidget(); // 내부에서 MainWidget->UpdateNewRound()
			}
		}
	}
}

void ABombGameState::HandleCurrentTurnChanged()
{
	if (UWorld* W = GetWorld())
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(W, 0))
		{
			if (ABombHUD* HUD = PC->GetHUD<ABombHUD>())
			{
				// TODO: 내차례 UI
				//HUD->UpdateMainWidget();
			}
		}
	}
}

