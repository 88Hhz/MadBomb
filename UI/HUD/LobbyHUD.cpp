// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LobbyHUD.h"

#include "BombGameInstance.h"
//#include "Session/SessionSubsystem.h"

#include "UI/Lobby/MainMenu.h"
#include "UI/Lobby/ServerMenu.h"
#include "UI/Lobby/LobbyWidget.h"
#include "UI/SettingWidget.h"


#include "Kismet/GameplayStatics.h"



ALobbyHUD::ALobbyHUD(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	/*ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBP(TEXT("'/Game/Lobby/UI/WBP_MainMenu.WBP_MainMenu_C'"));
	if (!ensure(MainMenuBP.Class != nullptr)) return;
	WBP_MainMenu = MainMenuBP.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> ServerMenuBP(TEXT("'/Game/Lobby/UI/WBP_ServerMenu.WBP_ServerMenu_C'"));
	if (!ensure(ServerMenuBP.Class != nullptr)) return;
	WBP_ServerMenu = ServerMenuBP.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidgetBP(TEXT("'/Game/Lobby/UI/WBP_LobbyWidget.WBP_LobbyWidget_C'"));
	if (!ensure(LobbyWidgetBP.Class != nullptr)) return;
	WBP_LobbyWidget = LobbyWidgetBP.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> SettingWidgetBP(TEXT("'/Game/UI/Setting/WBP_SettingWidget.WBP_SettingWidget_C'"));
	if (!ensure(SettingWidgetBP.Class != nullptr)) return;
	WBP_SettingWidget = SettingWidgetBP.Class;*/

}

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UBombGameInstance* GI = GetGameInstance<UBombGameInstance>())
	{
		if (!GI->IsInLobby())
		{
			ShowMainMenu();
		}
		else
		{
			ShowLobbyWidget();
		}
	}
}

void ALobbyHUD::ShowMainMenu()
{
	if (ServerMenu != nullptr)
	{
		ServerMenu->RemoveFromParent();
		ServerMenu = nullptr;
	}
	if (LobbyWidget != nullptr)
	{
		LobbyWidget->RemoveFromParent();
		LobbyWidget = nullptr;
	}

	if (!MainMenu)
	{
		if (!ensure(WBP_MainMenu != nullptr)) return;
		/*if (APlayerController* PC = GetOwningPlayerController())
		{
			
		}*/
		MainMenu = CreateWidget<UMainMenu>(GetWorld(), WBP_MainMenu);
		MainMenu->AddToViewport(1);
	}
}

void ALobbyHUD::ShowServerMenu()
{
	if (MainMenu != nullptr)
	{
		MainMenu->RemoveFromParent();
		MainMenu = nullptr;
	}
	if (!ServerMenu)
	{
		if (!ensure(WBP_ServerMenu != nullptr)) return;
		/*if (APlayerController* PC = GetOwningPlayerController())
		{
			
		}*/
		ServerMenu = CreateWidget<UServerMenu>(GetWorld(), WBP_ServerMenu);
		ServerMenu->AddToViewport(2);
	}
}

void ALobbyHUD::ShowLobbyWidget()
{
	if (ServerMenu != nullptr)
	{
		ServerMenu->RemoveFromParent();
		ServerMenu = nullptr;
	}

	if (!LobbyWidget)
	{
		/*if (APlayerController* PC = GetOwningPlayerController())
		{
			
		}*/
		LobbyWidget = CreateWidget<ULobbyWidget>(GetWorld(), WBP_LobbyWidget);
		LobbyWidget->AddToViewport(3);
	}
}

void ALobbyHUD::ShowSettingWidget()
{
	if (!SettingWidget)
	{
		/*if (APlayerController* PC = GetOwningPlayerController())
		{
			
		}*/
		SettingWidget = CreateWidget<USettingWidget>(GetWorld(), WBP_SettingWidget);
		SettingWidget->AddToViewport(2);
	}
}

void ALobbyHUD::CloseSettingWidget()
{
	if (SettingWidget != nullptr)
	{
		SettingWidget->RemoveFromParent();
		SettingWidget = nullptr;
	}
}

//void ALobbyHUD::AddLobbyList(const FString& NewName)
//{
//	if (LobbyWidget != nullptr)
//	{
//		LobbyWidget->AddPlayerList(NewName);
//	}
//}
//
//void ALobbyHUD::UpdateLobbyList(const TArray<FString>& PlayerNameList)
//{
//	if (LobbyWidget != nullptr)
//	{
//		LobbyWidget->UpdatePlayerList(PlayerNameList);
//	}
//}

void ALobbyHUD::EnableStartButton(bool CanStart)
{
	if (LobbyWidget != nullptr)
	{
		LobbyWidget->EnableStartButton(CanStart);
	}
}


