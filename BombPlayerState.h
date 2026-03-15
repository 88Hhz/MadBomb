// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "BombPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ABombPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ABombPlayerState();
	

protected:
	virtual void PostInitializeComponents() override;

	virtual void OverrideWith(class APlayerState* PlayerState) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game")
	bool bClientReady = false;

	void SetClientReady(bool bReady);


	void SetPlayerTurnOrder(int32 NewOrder);
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	int32 GetPlayerTurnOrder() const { return PlayerTurnOrder; }

	void SetHp(int32 NewHp);
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	int32 GetHp() { return Hp; }
	int32 GetMaxHp() { return MaxHp; }

	void SetCoin(int32 NewCoin) { Coin = NewCoin; }
	int32 GetCoin() { return Coin; }

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	bool GetIsAlive() { return bIsAlive; }
	

protected:
	UPROPERTY(Replicated)
	int32 PlayerTurnOrder = -1;

	UPROPERTY(Replicated)
	bool bIsAlive = true;

	UPROPERTY(ReplicatedUsing = OnRep_CheckHp)
	int32 Hp = 0;
	const int32 MaxHp = 20;

	UPROPERTY(Replicated)
	int32 Coin = 10;

private:
	UFUNCTION()
	void OnRep_CheckHp();
	
	// HP가 0일 때 캐릭터 죽음 처리 (서버에서만 호출됨)
	void HandleCharacterDeath();
	
	FTimerHandle DeathHideTimerHandle;

};
