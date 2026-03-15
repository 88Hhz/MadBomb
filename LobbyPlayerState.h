// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerState();

protected:
	virtual void PostInitializeComponents() override;
	virtual void SeamlessTravelTo(APlayerState* NewPlayerState) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Functions used in LobbyGameMode
	void SetReady(bool NewReady);
	bool GetIsReady() { return bIsReady; }


protected:
	UPROPERTY(ReplicatedUsing = OnRep_Ready)
	bool bIsReady = false;

	UFUNCTION()
	void OnRep_Ready();


private:

	/*UPROPERTY()
	UTexture2D* SteamAvatarTexture = nullptr;*/

};
