// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Session/SessionDataStruct.h"

#include "ServerMenu.generated.h"


/**
 * 
 */


UCLASS()
class SPINNINGBOMB_API UServerMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UServerMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	/*void SetServerInterface(IServerSettingInterface* SettingInterface) { this->ServerInterface = SettingInterface; }*/

	////ServerMenuInterface Functions
	//virtual void SetServerMenuInterface(IServerSettingInterface* SettingInterface) override { }
	//virtual void CreateSession(int32 NumPublicConnections, FString LobbyName, bool bFriendsOnly) override { }
	//virtual void FindCreatedSession() override { }
	//virtual void JoinCreatedSession(const uint32 Index) override { }
	//

	UFUNCTION()
	void SetLobbyList(TArray<struct FSessionDataStruct>& LobbyDatas);


	void SelectLobbyIndex(const uint32 Index, const FSessionDataStruct& LobbyData);


	//MainWidget
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "MainWidget")
	class UButton* CreateServerSettingBtt;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "MainWidget")
	class UButton* RandomMatchBtt;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "MainWidget")
	class UButton* RefreshListBtt;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "MainWidget")
	class UButton* BackBtt;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "MainWidget")
	class UPanelWidget* LobbyList;

	UFUNCTION(BlueprintCallable, category = "MainFunction")
	void OnClickedServerSetting();

	UFUNCTION(BlueprintCallable, category = "MainFunction")
	void OnClickedRandomMatch();

	UFUNCTION(BlueprintCallable, category = "MainFunction")
	void OnClickedRefreshList();

	UFUNCTION(BlueprintCallable, category = "MainFunction")
	void OnClickedBackMainMenu();

	//ServerSettingWidget
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "ServerSettingWidget")
	class UBorder* ServerSettingBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "ServerSettingWidget")
	class UButton* CloseServerSettingBtt;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "ServerSettingWidget")
	class UButton* CreateServerBtt;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), category = "ServerSettingWidget")
	class UEditableTextBox* LobbyNameTextBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), category = "ServerSettingWidget")
	class UTextBlock* NumConnectionsText;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), category = "ServerSettingWidget")
	class UCheckBox* IsPublicCheckBox;

	UPROPERTY(BlueprintReadWrite, category = "ServerSetting")
	int32 _NumConnections = 4;


	UPROPERTY(BlueprintReadWrite, category = "ServerSetting")
	bool _bIsFriendsOnly = false;

	UFUNCTION(BlueprintCallable, category = "ServerSettingFunction")
	void OnClickedCloseSetting();

	UFUNCTION(BlueprintCallable, category = "ServerSettingFunction")
	void OnClickedCreateServer();


	//JoinGameWidget
protected:
	UPROPERTY(meta = (BindWidget))
	class UBorder* JoinLobbyBorder;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* JoinLobbyName;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinLobbyBtt;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinCancelBtt;


	UFUNCTION(BlueprintCallable, category = "ServerSettingFunction")
	void OnClickedJoinLobby();

	UFUNCTION(BlueprintCallable, category = "ServerSettingFunction")
	void OnClickedJoinCancel();


private:
	//IServerSettingInterface* ServerInterface;

	UPROPERTY()
	TSubclassOf<class UUserWidget> ServerListRow;

	TOptional<int32> SelectedLobbyIndex;
	TOptional<struct FSessionDataStruct> SelectedLobbyData;


};
