// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/LobbyPlayerController.h"
#include "BombGameInstance.h"
#include "GameMode/LobbyGameMode.h"

#include "UI/HUD/LobbyHUD.h"
#include "Actor/Character/LobbyCharacter.h"
#include "LobbyPlayerState.h"

#include "GameFramework/PlayerState.h"


void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;
}

void ALobbyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

}

void ALobbyPlayerController::ExitCurrentLobby()
{
	if (UBombGameInstance* GI = GetGameInstance<UBombGameInstance>())
	{
		GI->DestroySession();
	}
}

//Only Access Server
void ALobbyPlayerController::StartGame()
{
	if (HasAuthority())
	{
		if (ALobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<ALobbyGameMode>())
		{
			GameMode->StartGame();
		}
	}
}

// For Initialize PlayerState Data
FString ALobbyPlayerController::GetUserName()
{
	UBombGameInstance* GI = GetGameInstance<UBombGameInstance>();
	if (!ensure(GI != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("LobbyPC: Casting GameInstance is Failed."));
	}
	return GI->GetUserName();

}

UTexture2D* ALobbyPlayerController::GetSteamAvatarTexture()
{
	if (!IsLocalController()) return nullptr;

	UBombGameInstance* GI = GetGameInstance<UBombGameInstance>();
	if (!ensure(GI != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("LobbyPC: Casting GameInstance is Failed."));
	}
	return GI->GetSteamAvatarTexture();

}


void ALobbyPlayerController::EnableStartButton(bool CanStart)
{
	if (ALobbyHUD* HUD = GetHUD<ALobbyHUD>())
	{
		HUD->EnableStartButton(CanStart);
	}
}

void ALobbyPlayerController::Server_UpdateReadyState_Implementation(bool NewReady)
{
	if (ALobbyPlayerState* PS = GetPlayerState<ALobbyPlayerState>())
	{
		PS->SetReady(NewReady);
	}
}


void ALobbyPlayerController::Server_GetPlayerList_Implementation()
{
	/*if (ALobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<ALobbyGameMode>())
	{
		GameMode->GetPlayerList(this);
	}*/
}

//
//void ALobbyPlayerController::Client_AddPlayerList_Implementation(const FString& NewName)
//{
//	ALobbyHUD* HUD = GetHUD<ALobbyHUD>();
//	if (HUD)
//	{
//		HUD->AddLobbyList(NewName);
//	}
//}
//
//
//void ALobbyPlayerController::Client_UpdatePlayerList_Implementation(const TArray<FString>& PlayerNameList)
//{
//	ALobbyHUD* HUD = GetHUD<ALobbyHUD>();
//	if (HUD)
//	{
//		HUD->UpdateLobbyList(PlayerNameList);
//	}
//}
