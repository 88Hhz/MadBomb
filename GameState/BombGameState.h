// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Actor/BombCardManagerComponent.h"
#include "BombGameState.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ABombGameState : public AGameState
{
	GENERATED_BODY()

public:
	ABombGameState();

protected:

	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//virtual void OnRep_ReplicatedHasBegunPlay() override;

public:
	void SetNumPlayersInGame(const int32 NumOfPlayer) { NumOfPlayerInGame = NumOfPlayer; }
	int32 GetNumPlayersInGame() const { return NumOfPlayerInGame; }

	void SetCurrentTurnOfPlayer(const int32 NewOrder);
	int32 GetCurrentTurnOfPlayer() const { return CurrentTurnPlayerOrder; }

	void SetMatchRound(const int32 NewRound);
	int32 GetMatchRound() const { return MatchRound; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CardFX")
	UBombCardManagerComponent* CardFX;

	UFUNCTION(BlueprintCallable, Category = "CardFX")
	void SetSeatTarget(int32 SeatIdx, AActor* TargetPoint);

	/** (선택) 중앙 포인트를 코드로 지정할 때 */
	UFUNCTION(BlueprintCallable, Category = "CardFX")
	void SetCenterPoint(AActor* Center);
protected:
	UPROPERTY(Replicated)
	int32 NumOfPlayerInGame = -1;

	//Currnet player index of turn
	UPROPERTY(ReplicatedUsing = OnRep_CurrentTurnPlayerOrder)
	int32 CurrentTurnPlayerOrder = -1; 

	UPROPERTY(ReplicatedUsing = OnRep_MatchRound)
	int32 MatchRound = -1;


private:
	UFUNCTION() 
	void OnRep_MatchRound();
	UFUNCTION() 
	void OnRep_CurrentTurnPlayerOrder();

	// 서버/클라 공용 처리(서버 Setter와 OnRep 둘 다에서 호출)
	void HandleMatchRoundChanged();
	void HandleCurrentTurnChanged();
};
