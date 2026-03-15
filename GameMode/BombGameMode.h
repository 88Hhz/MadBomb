// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Actor/Character/PlayerAnimInstance.h"
#include "BombGameMode.generated.h"

/**
 * 
 */
class ABomb;
class ABombCharacter;
class UCardManager;
class ABombPlayerController;
class ABombPlayerState;
class ASeatPoint;

UCLASS()
class SPINNINGBOMB_API ABombGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ABombGameMode();

protected:
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;

public:
    // chat system
    void SendChatMessage(const FString& Message);

    // Match
    UFUNCTION()
    void CheckAllClientsReady();
    void InitializeGameMatch();
    void InitializeNewRound();
    void StartNewRound();
    void CurrentRoundLogic();
    void EndCurrentRound();

    // 폭탄/카드
    void DealCardToCurrentPlayer();
    void SelectedCardFromPlayer(const FName& CardKey);
    void ReDrawCardsToPlayer();
    void ContinueCurrentRound();
    void ExplodeBomb(int32 damage);
    void Red_VisibleRemainingCount(int32 Count);

    // 카드 효과
    void Effect_MakeInverseOrder() { _bInverseOrder = !_bInverseOrder; }
    void Effect_JumpNextPlayer() { _bJumpTurn = true; }
    void Effect_BlockEffect() { _bBlockEffects = true; }
    void Effect_EliminateYellowCards() { /*TODO*/ }
    void Effect_MakeDealLessCardNextTurn() { _bDealLessCards = true; }

    UFUNCTION() 
    void OnBombArrivedAtSeat(int32 SeatIndex);
protected:
    UPROPERTY()
    TArray<ABombPlayerController*> PlayerControllers;

    UPROPERTY()
    TArray<ASeatPoint*> SpawnPoints;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ABomb> BombClass;

    UPROPERTY()
    ABomb* SpawnedBomb = nullptr;

    UPROPERTY()
    UCardManager* CardManager = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Character")
    TSubclassOf<ACharacter> BP_Player1;

    UPROPERTY(EditDefaultsOnly, Category = "Character")
    TSubclassOf<ACharacter> BP_Player2;

    UPROPERTY(EditDefaultsOnly, Category = "Character")
    TSubclassOf<ACharacter> BP_Player3;

    UPROPERTY(EditDefaultsOnly, Category = "AnimTiming")
    float EndRoundDelay = 3.5f;    // Win/Lose 보여줄 시간

private:
    bool AdvanceTurn(TArray<int32>& Path);
    int32 CountAlivePlayers() const;
    bool IsAliveAtIndex(int32 index) const;
    int32 NextDir() const { return _bInverseOrder ? -1 : 1; }

    ABombCharacter* GetCharacterAtSeat(int32 Seat) const;
    void PushStateToAll(EPlayerAnimState DefaultState, int32 Except);

    void ResetAllToIdle();
private:
    // 상태 플래그
    int32 ExpectedPlayers = 0; // 시작 목표 인원 (GI에서 읽음)
    bool  bGameStarted = false;
    bool  bRoundInProgress = false;
    bool  bResolvingRound = false;

    FTimerHandle RoundInitHandle;
    FTimerHandle EndRoundHandle;
    FTimerHandle ResetAnimHandle;

    // 좌석/라운드
    int32 SpawnPointIndex = 0;   // 순차 배정용
    int32 NumAssignedSeats = 0;   
    int32 NumOfPlayerInGame = 0;   // GI에서 읽은 값
    int32 NumOfPlayerInRound = 0;
    int32 MatchRound = 0;
    int32 StartIndexForNewRound = 0;
    int32 _curIndex = 0;

    // 카드 효과 플래그
    bool _bInverseOrder = false;
    bool _bJumpTurn = false;
    bool _bDealLessCards = false;
    bool _bBlockEffects = false;

    // 게임 종료 플래그
    bool bGameEnded = false;


};
