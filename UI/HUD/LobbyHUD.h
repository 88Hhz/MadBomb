// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "LobbyHUD.generated.h"


/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ALobbyHUD(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, category = "Widget Function")
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable, category = "Widget Function")
	void ShowServerMenu();

	UFUNCTION(BlueprintCallable, category = "Widget Function")
	void ShowLobbyWidget();

	UFUNCTION(BlueprintCallable, category = "Widget Function")
	void ShowSettingWidget();

	UFUNCTION(BlueprintCallable, category = "Widget Function")
	void CloseSettingWidget();

	/*UFUNCTION(category = "Widget Function")
	void AddLobbyList(const FString& NewName);

	UFUNCTION(category = "Widget Function")
	void UpdateLobbyList(const TArray<FString>& PlayerNameList);*/

	UFUNCTION(category = "Widget Function")
	void EnableStartButton(bool CanStart);


protected:
	UPROPERTY(BlueprintReadOnly, category = "Widget")
	class UMainMenu* MainMenu;

	UPROPERTY(BlueprintReadOnly, category = "Widget")
	class UServerMenu* ServerMenu;

	UPROPERTY(BlueprintReadOnly, category = "Widget")
	class ULobbyWidget* LobbyWidget;

	UPROPERTY(BlueprintReadOnly, category = "Widget")
	class USettingWidget* SettingWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "BP_Widget")
	TSubclassOf<class UMainMenu> WBP_MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "BP_Widget")
	TSubclassOf<class UServerMenu> WBP_ServerMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "BP_Widget")
	TSubclassOf<class ULobbyWidget> WBP_LobbyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "BP_Widget")
	TSubclassOf<class USettingWidget> WBP_SettingWidget;
private:
	

	UPROPERTY()
	bool bIsInLobby = false;


	/*UFUNCTION()
	void SetLobbyState(bool bJoinLobbySuccesful, FLobbyDataStruct& LobbyData);*/
};
