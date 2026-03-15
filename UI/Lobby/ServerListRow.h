// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Session/SessionDataStruct.h"

#include "ServerListRow.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UServerListRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LobbyName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUserName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerConnections;

	void SetLobbyData(const struct FSessionDataStruct InLobbyData);

	void SetIndex(class UServerMenu* InParent, int32 InIndex);

protected:
	

private:
	FSessionDataStruct LobbyData;

	UPROPERTY()
	class UServerMenu* Parent;

	int32 Index;

	UPROPERTY(meta = (BindWidget))
	class UButton* ListRowButton;

	UFUNCTION()
	void OnClickedLobbyRow();
};
