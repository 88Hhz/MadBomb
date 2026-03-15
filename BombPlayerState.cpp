// Fill out your copyright notice in the Description page of Project Settings.


#include "BombPlayerState.h"
#include "Net/UnrealNetwork.h"	//DOREPLIFETIME

#include "GameMode/BombGameMode.h"
#include "PlayerController/BombPlayerController.h"
#include "Actor/Character/BombCharacter.h"

#include "BombGameInstance.h"

//#include "Engine/Texture2D.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"


ABombPlayerState::ABombPlayerState()
{
	Hp = MaxHp;
}



void ABombPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UE_LOG(LogTemp, Warning, TEXT("PlayerStateName : %s"), *GetPlayerName());
}



void ABombPlayerState::OverrideWith(APlayerState* PlayerState)
{
	Super::OverrideWith(PlayerState);

	//������ ��
}

void ABombPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABombPlayerState, bClientReady);
	DOREPLIFETIME(ABombPlayerState, PlayerTurnOrder);
	DOREPLIFETIME(ABombPlayerState, Hp);
	DOREPLIFETIME(ABombPlayerState, bIsAlive);

}

void ABombPlayerState::SetClientReady(bool bReady)
{
	if (HasAuthority())
	{
		bClientReady = bReady;
	}
}

void ABombPlayerState::SetPlayerTurnOrder(int32 NewOrder)
{
	if (HasAuthority()) 
	{
		PlayerTurnOrder = NewOrder;
	}
}



void ABombPlayerState::SetHp(int32 NewHp)
{
	if (!HasAuthority()) return;

	bool bWasAlive = bIsAlive;
	Hp = NewHp;
	if (Hp <= 0) 
		bIsAlive = false;

	// HP가 0이 되었고, 이전에 살아있었다면 죽음 처리 (서버에서만)
	if (Hp <= 0 && bWasAlive && !bIsAlive)
	{
		HandleCharacterDeath();
	}

	OnRep_CheckHp();
}

void ABombPlayerState::HandleCharacterDeath()
{
	if (!HasAuthority()) return;
	
	// 서버에서만 실행
	if (ABombCharacter* Char = GetPawn<ABombCharacter>())
	{
		// 죽는 애니메이션 재생 (모두에게 전파됨 - Multicast_PushState 호출)
		Char->SetState_ServerAuth(EPlayerAnimState::Dead);
		
		// 2초 후 Hidden으로 전환 (서버에서 타이머 설정)
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(DeathHideTimerHandle);
			World->GetTimerManager().SetTimer(
				DeathHideTimerHandle,
				[Char]()
				{
					if (IsValid(Char) && Char->HasAuthority())
					{
						// 서버에서 Hidden 설정
						Char->SetActorHiddenInGame(true);
						Char->SetActorEnableCollision(false);
						
						// 메시 컴포넌트도 Hidden으로 설정
						if (Char->GetMesh())
						{
							Char->GetMesh()->SetVisibility(false, true);
						}
						// Widget도 Hidden
						if (UWidgetComponent* WidgetComp = Cast<UWidgetComponent>(
							Char->GetComponentByClass(UWidgetComponent::StaticClass())))
						{
							WidgetComp->SetVisibility(false);
						}
					}
				},
				2.0f,
				false
			);
		}
	}
}

void ABombPlayerState::OnRep_CheckHp()
{
	UE_LOG(LogTemp, Warning, TEXT("PS OnRep_Hp: %s -> Hp=%d"), *GetPlayerName(), Hp);

	if (ABombPlayerController* PC = Cast<ABombPlayerController>(GetPlayerController()))
	{
		// HP ��ȭ UI
		PC->Client_UpdateMainWidget();
	}

	if (ABombCharacter* Char = GetPawn<ABombCharacter>())
	{
		Char->UpdateHpWidget();
	}
}


