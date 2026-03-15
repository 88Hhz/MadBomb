// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


#include "LobbyWidget.generated.h"


UCLASS()
class SPINNINGBOMB_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:


	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	/*UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "PlayerNameWidget")
	void CheckPlayerList();

	void UpdatePlayerListWidget();*/

	/*void UpdatePlayerList(const TArray<FString>& PlayerNameList);
	void AddPlayerList(const FString& NewName);*/

	void EnableStartButton(bool CanStart);

protected:


protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "RoomWidget")
	class UTextBlock* LobbyName;


	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "PlayerNameWidget")
	class UButton* ExitLobbyBtt;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "PlayerNameWidget")
	class UButton* GameStartBtt;

	UPROPERTY()
	bool bIsReady = false;

	UFUNCTION()
	void OnClickedExitLobby();

	UFUNCTION()
	void OnClickedGameStart();

	//FLobbyDataStruct LobbyData;
private:
	FTimerHandle UpdateTimerHandle;


	/*UPROPERTY()
	TSubclassOf<UUserWidget> PlayerListRow;
	TArray<class ULobbyPlayerListRow*> PlayerArray;*/

};
