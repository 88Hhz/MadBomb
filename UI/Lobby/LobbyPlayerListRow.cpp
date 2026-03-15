// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyPlayerListRow.h"
#include "PlayerController/LobbyPlayerController.h"
#include "LobbyPlayerState.h"

#include "GameFramework/Character.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"



void ULobbyPlayerListRow::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Error, TEXT("ULobbyPlayerListRow::NATIVECONSTRUCT"));
	if (Char)
	{
		if (ALobbyPlayerState* PS = Cast<ALobbyPlayerState>(Char->GetPlayerState()))
		{
			UE_LOG(LogTemp, Error, TEXT("ULobbyPlayerListRow::SetNAme"));
			PlayerName->SetText(FText::FromString(PS->GetPlayerName()));
		}
	}
}


void ULobbyPlayerListRow::UpdatePlayerName()
{
	if (Char)
	{
		if (ALobbyPlayerState* PS = Cast<ALobbyPlayerState>(Char->GetPlayerState()))
		{
			UE_LOG(LogTemp, Error, TEXT("ULobbyPlayerListRow::UpdatePlayerName"));
			PlayerName->SetText(FText::FromString(PS->GetPlayerName()));
		}
	}
}

void ULobbyPlayerListRow::ReadyForGame()
{
	bIsReady = !bIsReady;
	if (bIsReady)
	{
		ReadyState->SetColorAndOpacity(FLinearColor::Green);
	}
	else
	{
		ReadyState->SetColorAndOpacity(FLinearColor::White);
	}
}
