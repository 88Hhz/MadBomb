// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BombPlayerController.h"
#include "GameMode/BombGameMode.h"
#include "BombPlayerState.h"
#include "GameState/BombGameState.h"
#include "UI/HUD/BombHUD.h"
#include "BombGameInstance.h"
#include "Actor/Character/BombCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "Kismet/GameplayStatics.h"



ABombPlayerController::ABombPlayerController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	/*static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_DEFAULT(TEXT("'/Game/ThirdPerson/Input/IMC_Default.IMC_Default'"));
	if (!ensure(IMC_DEFAULT.Object != nullptr)) return;

	MappingContext = IMC_DEFAULT.Object;*/
}

void ABombPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


}


void ABombPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

}

void ABombPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ABombPlayerController: BeginPlay()"));

	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;

	
}

void ABombPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Update all Widget for Client
	if (ABombHUD* HUD = GetHUD<ABombHUD>())
	{
		HUD->UpdateMainWidget();
	}
}




FString ABombPlayerController::GetUserName()
{
	UBombGameInstance* GI = GetGameInstance<UBombGameInstance>();
	if (!ensure(GI != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("BombPC: Casting GameInstance is Failed."));
	}
	return GI->GetUserName();

}

UTexture2D* ABombPlayerController::GetSteamAvatarTexture()
{
	if (!IsLocalController()) return nullptr;

	UBombGameInstance* GI = GetGameInstance<UBombGameInstance>();
	if (!ensure(GI != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("BombPC: Casting GameInstance is Failed."));
	}
	return GI->GetSteamAvatarTexture();
}


void ABombPlayerController::ActivateChatUI()
{
	if (ABombHUD* BombHUD = GetHUD<ABombHUD>())
	{
		BombHUD->ActivateChatUI();
		UE_LOG(LogTemp, Warning, TEXT("PlayerController: ActivateChatUI()"));
	}
}

void ABombPlayerController::Server_ReportClientReady_Implementation()
{
	if (auto* PS = GetPlayerState<ABombPlayerState>())
	{
		if (!PS->bClientReady) // 이미 true면 중복 무시
		{
			PS->bClientReady = true;
			if (auto* GM = GetWorld()->GetAuthGameMode<ABombGameMode>())
				GM->CheckAllClientsReady();
		}
	}
}


void ABombPlayerController::Client_UpdateMainWidget_Implementation()
{
	if (ABombHUD* BombHUD = GetHUD<ABombHUD>())
	{
		BombHUD->UpdateMainWidget();
		UE_LOG(LogTemp, Error, TEXT("BombPlayerController:: NotifyStartGame() Called."));
	}
}


//Chat System: Widget->PC->GM
void ABombPlayerController::Server_SendChatMessage_Implementation(const FString& Message)
{
	if (ABombPlayerState* BombPlayerState = this->GetPlayerState<ABombPlayerState>())
	{
		
		//FString NewMessage = FString::Printf(TEXT("%s: %s"), *BombPlayerState->GetPlayerName(), *Message);

		FString NewMessage;

		if (HasAuthority())
		{
			FString str = "88HZ";
			NewMessage = FString::Printf(TEXT("%s: %s"), &str, *Message);
		}
		else
		{
			FString str = "hhozae";
			NewMessage = FString::Printf(TEXT("%s: %s"), &str, *Message);
		}

		if (ABombGameMode* GameMode = GetWorld()->GetAuthGameMode<ABombGameMode>())
		{
			GameMode->SendChatMessage(NewMessage);
		}
	}
}

bool ABombPlayerController::Server_SendChatMessage_Validate(const FString& Message)
{
	return !Message.IsEmpty();
}


//Chat System:GM -> PC -> HUD -> Widget
void ABombPlayerController::Client_ReceiveChatMessage_Implementation(const FString& Message)
{
	if(ABombHUD* BombHUD = GetHUD<ABombHUD>())
	{
		BombHUD->AddChatMessage(Message);
	}
}


void ABombPlayerController::Client_ReceiveDealtCards_Implementation(const TArray<FName>& DealtCards)
{
	if(ABombHUD* BombHUD = GetHUD<ABombHUD>())
	{
		//BombHUD->SetCardWidgetWithDealtCard(DealtCards);
		BombHUD->PlayDealAndSetCards(DealtCards);
	}
	UE_LOG(LogTemp, Warning, TEXT("BombPlayerController : ReceiveCards : DealtCards : %d"), DealtCards.Num());
}


void ABombPlayerController::Server_SendSelectedCard_Implementation(const FName& CardKey)
{
	if (ABombGameMode* GM = GetWorld()->GetAuthGameMode<ABombGameMode>())
	{
		GM->SelectedCardFromPlayer(CardKey);
		UE_LOG(LogTemp, Error, TEXT("BombGameController::SendSelectedCard() Called."));
	}
}

bool ABombPlayerController::Server_SendSelectedCard_Validate(const FName& CardKey)
{
	UE_LOG(LogTemp, Warning, TEXT("BombGameController:Server_SendSelectedCard' CardKey : %s "), *CardKey.ToString());
	return CardKey.IsValid();
}

void ABombPlayerController::Server_ReDrawCards_Implementation()
{
	if (ABombGameMode* GM = GetWorld()->GetAuthGameMode<ABombGameMode>())
	{
		GM->ReDrawCardsToPlayer();
		UE_LOG(LogTemp, Error, TEXT("BombGameController::ReDrawCards() Called."));
	}
}

void ABombPlayerController::Client_VisibleRemainingCount_Implementation(const int32 Count)
{
	if (ABombHUD* BombHUD = GetHUD<ABombHUD>())
	{
		BombHUD->VisibleRemainingCount(Count);
	}
}

void ABombPlayerController::Client_ShowWinWidget_Implementation(int32 WinnerSeatIndex)
{
	// 클라이언트에서 자신의 캐릭터를 찾아서 위젯 표시
	if (ABombCharacter* BombChar = Cast<ABombCharacter>(GetCharacter()))
	{
		BombChar->ShowWinWidget(WinnerSeatIndex);
	}
}

