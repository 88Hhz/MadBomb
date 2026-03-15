// Fill out your copyright notice in the Description page of Project Settings.


#include "BombGameInstance.h"


#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

#include "UObject/ConstructorHelpers.h"

#include "Engine/Texture2D.h"
#include "Kismet/KismetRenderingLibrary.h"

#include "Kismet/GameplayStatics.h"

#define STEAM_APP_ID "480"

const static FName SESSION_NAME = TEXT("MadBomb");

UBombGameInstance::UBombGameInstance()
{

}

void UBombGameInstance::Init()
{
	Super::Init();

	//InitializeSteam();

    NumOfPlayerInGame = 0;
    CurrentLobbyName = "";

    if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
    {
        SessionInterface = OnlineSubsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBombGameInstance::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UBombGameInstance::OnDestroySessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBombGameInstance::OnFindSessionComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UBombGameInstance::OnJoinSessionComplete);
        }
        if (GEngine)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, FString::Printf(TEXT("Subsystem 이름: %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
        }

        if (GEngine != nullptr)
        {
            //추후 Network Failure 관리 추가
            GEngine->OnNetworkFailure().AddUObject(this, &UBombGameInstance::OnNetworkFailure);
        }

        if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
        {
            
        }
        else
        {
            InitializeSteam();
        }
    }
}

void UBombGameInstance::CreateSession(int32 NumPublicConnections, FString LobbyName, bool bFriendOnly)
{
    if (!SessionInterface.IsValid()) return;

    const TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());

    SessionSettings->bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;

    SessionSettings->bAllowInvites = true;							//Whether the match allows invitations for this session or not
    SessionSettings->bAllowJoinInProgress = true;					// Session 진행중에 접속 허용
    SessionSettings->bAllowJoinViaPresence = true;					// Presence를 통해 참여 허용

    SessionSettings->bUsesPresence = true;							// 현재 지역에 세션 표시
    
    SessionSettings->bUseLobbiesIfAvailable = true;					// 플랫폼이 지원하는 경우 로비 API 사용
    SessionSettings->NumPublicConnections = NumPublicConnections;	// 최대 접속 가능 수
    SessionSettings->bShouldAdvertise = !bFriendOnly;				// 현재 세션을 광고할지 (스팀의 다른 플레이어에게 세션 홍보 여부

    SessionSettings->Set(FName("LOBBY_NAME"), LobbyName, EOnlineDataAdvertisementType::ViaOnlineService);  // 로비의 이름 설정


    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), SESSION_NAME, *SessionSettings);

    //SessionInterface->CreateSession(0, SESSION_NAME, *SessionSettings);

    CurrentLobbyName = LobbyName;
}

void UBombGameInstance::DestroySession()
{
    if (!SessionInterface.IsValid()) return;

	if (SessionInterface->GetNamedSession(SESSION_NAME))
	{
		SessionInterface->DestroySession(SESSION_NAME);
		UE_LOG(LogTemp, Warning, TEXT("Destroying Session"));
	}
}

void UBombGameInstance::FindCreatedSession()
{
    if (!SessionInterface.IsValid()) return;

    SessionSearch = MakeShareable(new FOnlineSessionSearch());

    SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
    //SessionSearch->bIsLanQuery = false; 			//LAN을 사용할 경우 true
    SessionSearch->MaxSearchResults = 10000;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());

    //SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UBombGameInstance::JoinRandomSession()
{
    if (!SessionInterface.IsValid()) return;

    //추후 추가
}


void UBombGameInstance::JoinSelectedSession(const int32 Index)
{
    if (!SessionInterface.IsValid()) return;

    if (SessionSearch.IsValid())
    {
        SessionSearch->SearchResults[Index].Session.SessionSettings.Get(FName(TEXT("LOBBY_NAME")), CurrentLobbyName);

        const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
        SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), SESSION_NAME, SessionSearch->SearchResults[Index]);
        
        //SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
    }
}

void UBombGameInstance::StartSession()
{
    if (!SessionInterface.IsValid()) return;

    SessionInterface->StartSession(SESSION_NAME);
}



void UBombGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
    LoadMainMenu();
}

void UBombGameInstance::LoadMainMenu()
{
    if (APlayerController* PC = GetFirstLocalPlayerController()) {
        PC->ClientTravel("/Game/Levels/MainLevel", ETravelType::TRAVEL_Absolute);
    }

}


void UBombGameInstance::InitializeSteam()
{
	FString SteamAppIDPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries/Win64/Steam_appid.txt"));
	FFileHelper::SaveStringToFile(TEXT(STEAM_APP_ID), *SteamAppIDPath);

	/*if (SteamAPI_RestartAppIfNecessary(atoi(STEAM_APP_ID)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Steam requested restart."));
	}*/

	if (SteamAPI_Init())
	{
		SteamID = SteamUser()->GetSteamID();
        UserName = FText::FromString(UTF8_TO_TCHAR(SteamFriends()->GetPersonaName()));
		UE_LOG(LogTemp, Warning, TEXT("SteamInit(): %s"), *UserName.ToString());
		//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("SteamInit(): Name: %s"), *UserName.ToString()), true, true, FLinearColor::Green, 8.0f);

        SteamAvatarTexture = SetSteamAvatarTexture();
	}
}



UTexture2D* UBombGameInstance::SetSteamAvatarTexture()
{
    if (!SteamAPI_Init() || !SteamUser() || !SteamFriends())
    {
        UE_LOG(LogTemp, Error, TEXT("Steam API is not initialized"));
        return nullptr;
    }

    if (!SteamID.IsValid())
    {
        SteamID = SteamUser()->GetSteamID();
    }

    // 아바타 이미지 ID 가져오기
    int32 AvatarID = SteamFriends()->GetLargeFriendAvatar(SteamID);

    if (AvatarID == -1)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get Steam avatar ID"));
        return nullptr;
    }

    // 아바타 이미지 크기 확인 (Steam은 184x184의 Large 아바타 제공)
    uint32 Width, Height;
    if (!SteamUtils()->GetImageSize(AvatarID, &Width, &Height) || Width == 0 || Height == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid avatar image size"));
        return nullptr;
    }

    // Steam에서 이미지 데이터를 가져올 버퍼 생성
    TArray<uint8> AvatarRGBA;
    AvatarRGBA.SetNum(Width * Height * 4);  // RGBA 포맷이므로 픽셀당 4바이트

    // Steam에서 RGBA 데이터 가져오기
    if (!SteamUtils()->GetImageRGBA(AvatarID, AvatarRGBA.GetData(), AvatarRGBA.Num()))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get avatar image data"));
        return nullptr;
    }

    // Unreal의 UTexture2D로 변환
    UTexture2D* AvatarTexture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
    if (!AvatarTexture)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create UTexture2D"));
        return nullptr;
    }

    // 텍스처 데이터 업데이트
    FTexture2DMipMap& Mip = AvatarTexture->GetPlatformData()->Mips[0];
    void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(Data, AvatarRGBA.GetData(), AvatarRGBA.Num());
    Mip.BulkData.Unlock();

    AvatarTexture->UpdateResource();

    UE_LOG(LogTemp, Log, TEXT("Successfully created Steam Avatar Texture"));
    return AvatarTexture;
}


void UBombGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        bIsInLobby = true;
        GetWorld()->ServerTravel("/Game/Levels/LobbyMap?listen");
    }
}


void UBombGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        bIsInLobby = false;
        LoadMainMenu();
    }
}

void UBombGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
    if (!SessionInterface.IsValid() || !bWasSuccessful)	return;


    TArray<FSessionDataStruct> SessionDatas;
    if (SessionSearch->SearchResults.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session search completed but no results found."));
    }
    else 
    {
        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            FSessionDataStruct SessionData;
            SearchResult.Session.SessionSettings.Get(FName(TEXT("LOBBY_NAME")), SessionData.LobbyName);
            SessionData.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
            SessionData.CurrentPlayers = SessionData.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;

            SessionData.bIsFriendsOnly = SearchResult.Session.SessionSettings.bShouldAdvertise;

            SessionData.HostUserName = SearchResult.Session.OwningUserName;
            SessionDatas.Add(SessionData);
            UE_LOG(LogTemp, Warning, TEXT("%s"), *SessionData.LobbyName);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Found %d sessions"), SessionSearch->SearchResults.Num());
    OnFindSessionCompleted.ExecuteIfBound(SessionDatas);
}

void UBombGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (!SessionInterface.IsValid()) return;

    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        bIsInLobby = true;
        FString Address = "";
        SessionInterface->GetResolvedConnectString(SESSION_NAME, Address);
        if (Address != "")
        {
            PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
        }
    }
}
