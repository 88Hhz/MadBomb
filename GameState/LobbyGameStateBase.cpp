// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/LobbyGameStateBase.h"
#include "GameMode/LobbyGameMode.h"

#include "PlayerController/LobbyPlayerController.h"

#include "Net/UnrealNetwork.h"	//DOREPLIFETIME


ALobbyGameStateBase::ALobbyGameStateBase()
{
	
}

void ALobbyGameStateBase::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

}

void ALobbyGameStateBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ALobbyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameStateBase, NumOfLobbyPlayer);
}

void ALobbyGameStateBase::PostNetReceive()
{
	Super::PostNetReceive();
	
	/*if (OnPlayerArrayUpdated.IsBound())
	{
		OnPlayerArrayUpdated.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("GameStateBase:: PostNetReceive and Broadcast"));
	}*/
}

void ALobbyGameStateBase::SetNumOfLobbyPlayer(const int32 NumOfPlayer)
{
	NumOfLobbyPlayer = NumOfPlayer;

	if (HasAuthority())
	{
		
	}
}

void ALobbyGameStateBase::OnRep_LobbyPlayer()
{
	
}
