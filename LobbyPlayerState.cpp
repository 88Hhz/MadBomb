// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"	//DOREPLIFETIME

#include "GameMode/LobbyGameMode.h"
#include "PlayerController/LobbyPlayerController.h"
#include "Actor/Character/LobbyCharacter.h"

#include "BombGameInstance.h"

//#include "Engine/Texture2D.h"

#include "Kismet/GameplayStatics.h"

ALobbyPlayerState::ALobbyPlayerState()
{

}

void ALobbyPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetPlayerController()))
	{
		SetPlayerName(PC->GetUserName());
	}

}

void ALobbyPlayerState::SeamlessTravelTo(APlayerState* NewPlayerState)
{
	Super::SeamlessTravelTo(NewPlayerState);

}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, bIsReady);

}

void ALobbyPlayerState::SetReady(bool NewReady)
{
	if (HasAuthority())
	{
		bIsReady = NewReady;
		if (ALobbyGameMode* GM = GetWorld()->GetAuthGameMode<ALobbyGameMode>())
		{
			GM->CheckAllPlayerReadyState();
		}
	}

	OnRep_Ready();
}

void ALobbyPlayerState::OnRep_Ready()
{
	if (APawn* P = GetPawn())
	{
		if (ALobbyCharacter* LC = Cast<ALobbyCharacter>(P))
		{
			// Update... Character WidgetComponent 
			LC->UpdateReadyState();
		}
	}
}

