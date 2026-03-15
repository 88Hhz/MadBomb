// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyWidget.h"
#include "UI/Lobby/LobbyPlayerListRow.h"

#include "BombGameInstance.h"
#include "PlayerController/LobbyPlayerController.h"
#include "GameState/LobbyGameStateBase.h"

#include "BombPlayerState.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

#include "Kismet/GamePlayStatics.h"




void ULobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ensure(ExitLobbyBtt != nullptr)) return;
	ExitLobbyBtt->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickedExitLobby);
	
	if (!ensure(GameStartBtt != nullptr)) return;
	GameStartBtt->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickedGameStart);
}

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (UBombGameInstance* GI = GetGameInstance<UBombGameInstance>())
	{
		LobbyName->SetText(FText::FromString(GI->GetCurrentLobbyName()));
	}


	if(APlayerController* PC = GetOwningPlayer())
	{
		//Game Start Buttom
		GameStartBtt->SetIsEnabled(!PC->HasAuthority());
		if (PC->HasAuthority())
		{
			GameStartBtt->SetVisibility(ESlateVisibility::Hidden);
			//CheckPlayerList();
		}
		else
		{
			GameStartBtt->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ULobbyWidget::NativeDestruct()
{

}

void ULobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}


//void ULobbyWidget::UpdatePlayerList(const TArray<FString>& PlayerNameList)
//{
//	if (!ensure(PlayerList != nullptr)) return;
//	PlayerList->ClearChildren();
//
//	for (FString Name : PlayerNameList)
//	{
//		ULobbyPlayerListRow* ListRow = CreateWidget<ULobbyPlayerListRow>(this, PlayerListRow);
//		//ListRow->SetPlayerList(Name);
//
//		PlayerList->AddChild(ListRow);
//	}
//}
//
//void ULobbyWidget::AddPlayerList(const FString& NewName)
//{
//	if (!ensure(PlayerList != nullptr)) return;
//
//	ULobbyPlayerListRow* ListRow = CreateWidget<ULobbyPlayerListRow>(this, PlayerListRow);
//	//ListRow->SetPlayerList(NewName);
//
//	PlayerList->AddChild(ListRow);
//}

void ULobbyWidget::EnableStartButton(bool CanStart)
{
	if (!CanStart)
	{
		GameStartBtt->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		GameStartBtt->SetVisibility(ESlateVisibility::Visible);
	}
	GameStartBtt->SetIsEnabled(CanStart);
}



void ULobbyWidget::OnClickedExitLobby()
{
	if(ALobbyPlayerController* Controller = GetOwningPlayer<ALobbyPlayerController>())
	{
		Controller->ExitCurrentLobby();
	}
}

void ULobbyWidget::OnClickedGameStart()
{
	if (ALobbyPlayerController* PC = GetOwningPlayer<ALobbyPlayerController>())
	{
		//같은 버튼이 클라이언트는 Ready
		if (!PC->HasAuthority())
		{
			bIsReady = !bIsReady;
			PC->Server_UpdateReadyState(bIsReady);
		}
		//서버는 GameStart 버튼
		else
		{
			PC->StartGame();
		}
	}
}
