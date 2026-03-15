// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "LobbyGameStateBase.generated.h"


/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ALobbyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	

public:
	ALobbyGameStateBase();

protected:
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostNetReceive() override;	

public:
	void SetNumOfLobbyPlayer(const int32 NumOfPlayer);
	//void SetLobbyInterface(ILobbyWidgetInterface* Interface);

	int32 GetNumOfLobbyPlayer() { return NumOfLobbyPlayer; }


protected:
	UPROPERTY(ReplicatedUsing = OnRep_LobbyPlayer)
	int32 NumOfLobbyPlayer = 0;

	UFUNCTION()
	void OnRep_LobbyPlayer();



private:


};
