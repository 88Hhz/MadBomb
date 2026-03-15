// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/ServerMenu.h"

#include "BombGameInstance.h"
#include "UI/HUD/LobbyHUD.h"

#include "UI/Lobby/ServerListRow.h"


#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"




UServerMenu::UServerMenu(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerListRowBP(TEXT("'/Game/Lobby/UI/WBP_ServerListRow.WBP_ServerListRow_C'"));
	if (!ensure(ServerListRowBP.Class != nullptr)) return;

	ServerListRow = ServerListRowBP.Class;
}



void UServerMenu::NativeOnInitialized()
{
	Super::OnInitialized();

	if (!ensure(CreateServerSettingBtt != nullptr)) return;
	CreateServerSettingBtt->OnClicked.AddDynamic(this, &UServerMenu::OnClickedServerSetting);

	if (!ensure(RandomMatchBtt != nullptr)) return;
	RandomMatchBtt->OnClicked.AddDynamic(this, &UServerMenu::OnClickedRandomMatch);

	if (!ensure(RefreshListBtt != nullptr)) return;
	RefreshListBtt->OnClicked.AddDynamic(this, &UServerMenu::OnClickedRefreshList);

	if (!ensure(BackBtt != nullptr)) return;
	BackBtt->OnClicked.AddDynamic(this, &UServerMenu::OnClickedBackMainMenu);

	if (!ensure(CloseServerSettingBtt != nullptr)) return;
	CloseServerSettingBtt->OnClicked.AddDynamic(this, &UServerMenu::OnClickedCloseSetting);

	if (!ensure(CreateServerBtt != nullptr)) return;
	CreateServerBtt->OnClicked.AddDynamic(this, &UServerMenu::OnClickedCreateServer);
}

void UServerMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!ensure(ServerSettingBorder != nullptr)) return;
	ServerSettingBorder->SetVisibility(ESlateVisibility::Hidden);

	if (!ensure(LobbyNameTextBox != nullptr)) return;
	if (!ensure(NumConnectionsText != nullptr)) return;
	if (!ensure(IsPublicCheckBox != nullptr)) return;

	if (!ensure(JoinLobbyBorder != nullptr)) return;
	JoinLobbyBorder->SetVisibility(ESlateVisibility::Hidden);

	if (UBombGameInstance* GI = GetGameInstance<UBombGameInstance>())
	{
		GI->OnFindSessionCompleted.BindUFunction(this, FName("SetLobbyList"));
	}
}

void UServerMenu::NativeDestruct()
{

}

// GameInstance의 델리게이트 호출 시,
void UServerMenu::SetLobbyList(TArray<FSessionDataStruct>& LobbyDatas)
{
	LobbyList->ClearChildren();

	if (!LobbyDatas.IsEmpty())
	{
		uint32 index = 0;
		for (const FSessionDataStruct& LobbyData : LobbyDatas)
		{
			UServerListRow* ListRow = CreateWidget<UServerListRow>(this, ServerListRow);
			if (!ensure(ListRow != nullptr)) return;

			ListRow->SetLobbyData(LobbyData);
			ListRow->SetIndex(this, index++);
			LobbyList->AddChild(ListRow);
		}
	}
}

void UServerMenu::SelectLobbyIndex(const uint32 Index, const FSessionDataStruct& LobbyData)
{
	SelectedLobbyIndex = Index;
	SelectedLobbyData = LobbyData;

	this->JoinLobbyName->SetText(FText::FromString(LobbyData.LobbyName));

	if (!ensure(JoinLobbyBtt != nullptr)) return;
	JoinLobbyBtt->OnClicked.AddDynamic(this, &UServerMenu::OnClickedJoinLobby);

	if (!ensure(JoinCancelBtt != nullptr)) return;
	JoinCancelBtt->OnClicked.AddDynamic(this, &UServerMenu::OnClickedJoinCancel);

	JoinLobbyBorder->SetVisibility(ESlateVisibility::Visible);
}



void UServerMenu::OnClickedServerSetting()
{
	ServerSettingBorder->SetVisibility(ESlateVisibility::Visible);
}

void UServerMenu::OnClickedRandomMatch()
{
}

void UServerMenu::OnClickedRefreshList()
{
	if (UBombGameInstance* GI = GetGameInstance<UBombGameInstance>())
	{
		GI->FindCreatedSession();
	}
}

void UServerMenu::OnClickedBackMainMenu()
{
	if (APlayerController* Controller = Cast<APlayerController>(GetOwningPlayer()))
	{
		if (ALobbyHUD* HUD = Cast<ALobbyHUD>(Controller->GetHUD()))
		{
			HUD->ShowMainMenu();
		}
	}
}

void UServerMenu::OnClickedCloseSetting()
{
	ServerSettingBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UServerMenu::OnClickedCreateServer()
{
	FString TextBox = LobbyNameTextBox->GetText().ToString();
	FString LobbyName = !TextBox.IsEmpty() ? TextBox : FString("DefaultName");

	if (UBombGameInstance* GI = GetGameInstance<UBombGameInstance>())
	{
		GI->CreateSession(_NumConnections, LobbyName, _bIsFriendsOnly);
	}
}

void UServerMenu::OnClickedJoinLobby()
{
	if (SelectedLobbyData.IsSet())
	{
		if (UBombGameInstance* GI = GetGameInstance<UBombGameInstance>())
		{
			GI->JoinSelectedSession(SelectedLobbyIndex.GetValue());
		}
	}
}

void UServerMenu::OnClickedJoinCancel()
{
	JoinLobbyBorder->SetVisibility(ESlateVisibility::Hidden);
	//JoinLobbyBtt->OnClicked.RemoveDynamic(this, &UServerMenu::OnClickedJoinLobby);
	JoinCancelBtt->OnClicked.RemoveDynamic(this, &UServerMenu::OnClickedJoinCancel);
}
