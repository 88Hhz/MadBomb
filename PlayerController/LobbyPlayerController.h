// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:


protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;

public:
	UFUNCTION()
	void ExitCurrentLobby();

	//void LeaveLobbyCompleted(bool bWasSuccessful);
	void StartGame();

	FString GetUserName();

	UTexture2D* GetSteamAvatarTexture();

	UFUNCTION(Server, Reliable)
	void Server_UpdateReadyState(bool NewReady);

	UFUNCTION()
	void EnableStartButton(bool CanStart);


	UFUNCTION(Server, Reliable)
	void Server_GetPlayerList();

	//UFUNCTION(Client, Reliable)
	//void Client_AddPlayerList(const FString& NewName);

	//UFUNCTION(Client, Reliable)
	//void Client_UpdatePlayerList(const TArray<FString>& PlayerNameList);


protected:


private:

};
