// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/ServerListRow.h"
#include "UI/Lobby/ServerMenu.h"



#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"

void UServerListRow::SetLobbyData(const FSessionDataStruct InLobbyData)
{
	//UKismetSystemLibrary::PrintString(this, TEXT("on"), true, true, FLinearColor::Green, 0.5f);
	this->LobbyData = InLobbyData;

	LobbyName->SetText(FText::FromString(LobbyData.LobbyName));
	//GameMode->SetText(FText::FromString(LobbyData.GameMode));
	HostUserName->SetText(FText::FromString(LobbyData.HostUserName));

	FString ConnectionsRatio = FString::Printf(TEXT("%d/%d"), LobbyData.CurrentPlayers, LobbyData.MaxPlayers);
	PlayerConnections->SetText(FText::FromString(ConnectionsRatio));
}

void UServerListRow::SetIndex(UServerMenu* InParent, int32 InIndex)
{
	this->Parent = InParent;
	this->Index = InIndex;
	ListRowButton->OnClicked.AddDynamic(this, &UServerListRow::OnClickedLobbyRow);
}

void UServerListRow::OnClickedLobbyRow()
{
	Parent->SelectLobbyIndex(Index, LobbyData);
}

