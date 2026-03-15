// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#pragma warning(push)
#pragma warning(disable:4996)
#include "ThirdParty/Steamworks/Steamv157/sdk/public/steam/steam_api.h"
#pragma warning(pop)
//#pragma comment(lib, "ThirdParty/Steamworks/Steamv161/sdk/redistributable_bin/win64/steam_api64.lib")

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Session/SessionDataStruct.h"
#include "BombGameInstance.generated.h"



DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFindSessionCompleted, TArray<FSessionDataStruct>&, SessionDatas);

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UBombGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UBombGameInstance();

protected:
	virtual void Init() override;


public:
	void CreateSession(int32 NumPublicConnections, FString LobbyName, bool bFriendOnly);
	void DestroySession();
	void FindCreatedSession();
	void JoinRandomSession();
	void JoinSelectedSession(const int32 Index);

	void StartSession();

	FString GetCurrentLobbyName() { return this->CurrentLobbyName; }

	UFUNCTION(BlueprintCallable, category = "UserData")
	FString GetUserName() { return UserName.ToString(); }

	UFUNCTION(BlueprintCallable, category = "UserData")
	UTexture2D* GetSteamAvatarTexture() { return SteamAvatarTexture; }

	void SetNumOfPlayerInGame(int32 NewNum) { this->NumOfPlayerInGame = NewNum; }
	int32 GetNumOfPlayerInGame() { return this->NumOfPlayerInGame; }

	bool IsInLobby() { return bIsInLobby; }

	UPROPERTY()
	FOnFindSessionCompleted OnFindSessionCompleted;

protected:
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void LoadMainMenu();

private:
	void InitializeSteam();

	UFUNCTION()
	UTexture2D* SetSteamAvatarTexture();

	//static constexpr char* APP_ID = STEAM_APP_ID;
	CSteamID SteamID;

	UPROPERTY()
	FText UserName;

	UPROPERTY()
	UTexture2D* SteamAvatarTexture = nullptr;

	int32 NumOfPlayerInGame = 0;
	bool bIsInLobby = false;
	FString CurrentLobbyName = "";

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
