// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BombPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ABombPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABombPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;

public:
	FString GetUserName();
	UTexture2D* GetSteamAvatarTexture();

	UFUNCTION(BlueprintCallable)
	void ActivateChatUI();

	UFUNCTION(Server, Reliable)
	void Server_ReportClientReady();


	UFUNCTION(Client, Reliable)
	void Client_UpdateMainWidget();
	void Client_UpdateMainWidget_Implementation();



	//Chat Service
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendChatMessage(const FString& Message);
	bool Server_SendChatMessage_Validate(const FString& Message);

	UFUNCTION(Client, Reliable)
	void Client_ReceiveChatMessage(const FString& Message);



	//InGame Card Deal System/ GM -> PC -> HUD -> CardWidget
	UFUNCTION(Client, Reliable)
	void Client_ReceiveDealtCards(const TArray<FName>& DealtCards);


	//Player selected Card Info / CardWidget -> PC -> GM
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendSelectedCard(const FName& CardKey);
	void Server_SendSelectedCard_Implementation(const FName& CardKey);
	bool Server_SendSelectedCard_Validate(const FName& CardKey);

	UFUNCTION(Server, Reliable)
	void Server_ReDrawCards();

	UFUNCTION(Client, Reliable)
	void Client_VisibleRemainingCount(const int32 Count);

	UFUNCTION(Client, Reliable)
	void Client_ShowWinWidget(int32 WinnerSeatIndex);
	void Client_ShowWinWidget_Implementation(int32 WinnerSeatIndex);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<AHUD> HUDClass;


private:
	//class UInputMappingContext* MappingContext;

};
