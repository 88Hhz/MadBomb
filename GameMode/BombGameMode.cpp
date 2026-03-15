#include "GameMode/BombGameMode.h"
#include "BombGameInstance.h"
#include "GameState/BombGameState.h"
#include "PlayerController/BombPlayerController.h"
#include "BombPlayerState.h"
#include "Actor/Bomb/Bomb.h"
#include "Object/Card/CardManager.h"
#include "Actor/Character/BombCharacter.h"

#include "Actor/Character/SeatPoint.h"
#include "Actor/BombCardManagerComponent.h" 

#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

ABombGameMode::ABombGameMode()
{
    PlayerControllerClass = ABombPlayerController::StaticClass();
    GameStateClass = ABombGameState::StaticClass();
    PlayerStateClass = ABombPlayerState::StaticClass();
    bDelayedStart = true;
}

void ABombGameMode::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    SpawnPoints.Empty();
    for (TActorIterator<ASeatPoint> It(GetWorld()); It; ++It)
    {
        if (ASeatPoint* Seat = *It)
        {
            if (SpawnPoints.Num() <= Seat->SeatIndex)
                SpawnPoints.SetNum(Seat->SeatIndex + 1);

            SpawnPoints[Seat->SeatIndex] = Seat;
        }
    }

    PlayerControllers.SetNum(SpawnPoints.Num());

    if (UBombGameInstance* GI = GetGameInstance<UBombGameInstance>())
    {
        ExpectedPlayers = GI->GetNumOfPlayerInGame();
        NumOfPlayerInGame = ExpectedPlayers;
    }

    if (ABombGameState* GS = GetGameState<ABombGameState>())
    {
        GS->SetNumPlayersInGame(NumOfPlayerInGame);
        GS->SetMatchRound(0);                 // 아직 라운드 시작 전이면 0으로
        GS->SetCurrentTurnOfPlayer(-1);       // 아직 누구 차례도 아님
    }

    SpawnPointIndex = 0;


    if (ABombGameState* GS = GetGameState<ABombGameState>())
    {
        if (UBombCardManagerComponent* FX = GS->FindComponentByClass<UBombCardManagerComponent>())
        {
            for (int32 i = 0; i < SpawnPoints.Num(); ++i)
            {
                if (SpawnPoints[i])
                {
                    FSeatTarget Data;
                    Data.TargetPoint = SpawnPoints[i];
                    FX->SeatTargets.Add(i, Data);
                }
            }
            // CenterLocation은 (3)에서 폭탄 스폰 시점에 매번 갱신됨
        }
    }
}



void ABombGameMode::BeginPlay()
{
    Super::BeginPlay();
}


void ABombGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    ABombPlayerController* PC = Cast<ABombPlayerController>(NewPlayer);
    ABombPlayerState* PS = PC ? Cast<ABombPlayerState>(PC->PlayerState) : nullptr;
    if (!PC || !PS) return;

    int32 SeatIdx = NumAssignedSeats;
    if (ExpectedPlayers == 2)
        SeatIdx = (NumAssignedSeats == 0) ? 1 : 3;

    PS->SetPlayerTurnOrder(SeatIdx);

    if (PlayerControllers.Num() <= SeatIdx)
        PlayerControllers.SetNum(SeatIdx + 1);
    PlayerControllers[SeatIdx] = PC;

    TSubclassOf<ACharacter> ChClass = nullptr;
    if (NumAssignedSeats == 0)      ChClass = BP_Player1;
    else if (NumAssignedSeats == 1) ChClass = BP_Player2;
    else                            ChClass = BP_Player3;

    if (SpawnPoints.IsValidIndex(SeatIdx))
    {
        const FTransform& Tr = SpawnPoints[SeatIdx]->GetActorTransform();
        if (ACharacter* C = GetWorld()->SpawnActor<ACharacter>(ChClass, Tr))
        {
            PC->Possess(C);

            const FRotator Rot = Tr.GetRotation().Rotator();
            C->bUseControllerRotationYaw = true;
            PC->SetControlRotation(Rot);
            PC->ClientSetRotation(Rot, true);

            if (ABombCharacter* BC = Cast<ABombCharacter>(C))
            {
                BC->SetState_ServerAuth(EPlayerAnimState::Idle);
            }
        }
        ++NumAssignedSeats;
    }

    CheckAllClientsReady();
}




void ABombGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    PlayerControllers.Remove(Cast<ABombPlayerController>(Exiting));
    CheckAllClientsReady();
}



void ABombGameMode::SendChatMessage(const FString& Message)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        ABombPlayerController* PC = Cast<ABombPlayerController>(*It);
        if (PC)
        {
            PC->Client_ReceiveChatMessage(Message);
        }
    }
}


void ABombGameMode::CheckAllClientsReady()
{
    if (!HasAuthority() || bGameStarted) return;
    if (ExpectedPlayers <= 0) return;

    int32 total = 0, ready = 0;
    for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
    {
        if (auto* pc = Cast<ABombPlayerController>(it->Get()))
        {
            ++total;
            if (auto* ps = pc->GetPlayerState<ABombPlayerState>())
                ready += ps->bClientReady ? 1 : 0;
        }
    }

    if (total == ExpectedPlayers && ready == ExpectedPlayers)
    {
        UE_LOG(LogTemp, Log, TEXT("All ready: %d/%d"), ready, ExpectedPlayers);
        bGameStarted = true;
        InitializeGameMatch();
    }
}


void ABombGameMode::InitializeGameMatch()
{
    if (!HasAuthority()) return;

    MatchRound = 0;
    StartIndexForNewRound = 0;
    InitializeNewRound();
}

void ABombGameMode::InitializeNewRound()
{
    if (!HasAuthority()) return;
    
    // 게임이 종료되면 폭탄 생성하지 않음
    if (bGameEnded)
    {
        UE_LOG(LogTemp, Warning, TEXT("Game ended - skipping bomb spawn"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("InitializeNewRound"));

    if (SpawnedBomb) 
    {
        SpawnedBomb->Destroy(); 
        SpawnedBomb = nullptr;
    }
    if (CardManager) 
    {
        CardManager->ConditionalBeginDestroy(); 
        CardManager = nullptr; 
    }

    ++MatchRound;
    NumOfPlayerInRound = CountAlivePlayers();
    _curIndex = StartIndexForNewRound;

    if (ABombGameState* GS = GetGameState<ABombGameState>())
    {
        GS->SetMatchRound(MatchRound);
        GS->SetCurrentTurnOfPlayer(_curIndex);
    }

    PushStateToAll(EPlayerAnimState::Idle, -1);

    if (BombClass)
    {
        const FVector Loc(1450.f, 1850.f, 85.f);
        SpawnedBomb = GetWorld()->SpawnActor<ABomb>(BombClass, Loc, FRotator::ZeroRotator);

        // 카드 위치 시작점 
        if (ABombGameState* GS = GetGameState<ABombGameState>())
        {
            if (UBombCardManagerComponent* FX = GS->FindComponentByClass<UBombCardManagerComponent>())
            {
                FX->SetCenterLocation(Loc);
            }
        }
    }

    CardManager = NewObject<UCardManager>(this);
    if (CardManager) CardManager->InitializeCardPool();

    _bInverseOrder = false;
    _bJumpTurn = false;
    _bDealLessCards = false;
    _bBlockEffects = false;

    bRoundInProgress = true;

    //StartNewRound();

    // 클라이언트 bomb raised 대기
    GetWorld()->GetTimerManager().ClearTimer(RoundInitHandle);
    GetWorld()->GetTimerManager().SetTimer(
        RoundInitHandle, this, &ABombGameMode::StartNewRound, 2.f, false
    );
}

void ABombGameMode::StartNewRound()
{
    UE_LOG(LogTemp, Warning, TEXT("StartNewRound()"));

    NumOfPlayerInRound = CountAlivePlayers();
    if (NumOfPlayerInRound <= 1)
    {
        EndCurrentRound();
        return;
    }

    if (!IsAliveAtIndex(_curIndex))
    {
        TArray<int32> Path;
        if (!AdvanceTurn(Path))
        {
            EndCurrentRound();
            return;
        }
        _curIndex = Path.Last();
    }

    if (ABombGameState* GS = GetGameState<ABombGameState>())
    {
        GS->SetCurrentTurnOfPlayer(_curIndex);
    }

    //PushStateToAll(EPlayerAnimState::Idle, _curIndex);

    if (SpawnedBomb)
        SpawnedBomb->MoveInNewRound(_curIndex);
}

void ABombGameMode::CurrentRoundLogic()
{
    TArray<int32> Path;
    if (!AdvanceTurn(Path))
    {
        EndCurrentRound();
        return;
    }

    if (ABombGameState* GS = GetGameState<ABombGameState>())
    {
        GS->SetCurrentTurnOfPlayer(_curIndex);
    }

    if (SpawnedBomb)
        SpawnedBomb->PlayNextPath(Path);

    //PushStateToAll(EPlayerAnimState::Idle, _curIndex);
}


void ABombGameMode::EndCurrentRound()
{
    if (!HasAuthority()) return;

    UE_LOG(LogTemp, Warning, TEXT("EndCurrentRound"));

    bRoundInProgress = false;
    StartIndexForNewRound = _curIndex;

    // 안전: 혹시 남아있으면 파괴
    if (SpawnedBomb)
    {
        SpawnedBomb->Destroy();
        SpawnedBomb = nullptr;
    }
    if (CardManager)
    {
        CardManager->ConditionalBeginDestroy();
        CardManager = nullptr;
    }

    // 이전 스케줄 지우고, 다음 라운드 예약 (중복 예약 방지)
    GetWorld()->GetTimerManager().ClearTimer(RoundInitHandle);
    GetWorld()->GetTimerManager().SetTimer(
        RoundInitHandle, this, &ABombGameMode::InitializeNewRound, 4.0f, false
    );

    bResolvingRound = false; // 종료 처리 해제
}


void ABombGameMode::DealCardToCurrentPlayer()
{
    if (!PlayerControllers.IsValidIndex(_curIndex)) return;

    if (auto* PC = PlayerControllers[_curIndex])
    {
        if (CardManager)
        {
            int32 num = _bDealLessCards ? 2 : 3;
            _bDealLessCards = false;

            // 기존: 실제 카드 뽑기 + UI로 전달 (유지)
            PC->Client_ReceiveDealtCards(CardManager->DealCards(num));

            if (ABombGameState* GS = GetGameState<ABombGameState>())
            {
                if (GS->CardFX)
                {
                    GS->CardFX->Server_FlyCardToSeat(_curIndex, 1.f);
                }
            }
        }
    }
}


void ABombGameMode::SelectedCardFromPlayer(const FName& CardKey)
{
    if (SpawnedBomb)
        SpawnedBomb->ApplyCardEffect(CardKey);

    PushStateToAll(EPlayerAnimState::Idle, -1);
}

void ABombGameMode::ReDrawCardsToPlayer()
{
    if (auto* PC = PlayerControllers[_curIndex])
    {
        if (CardManager)
        {
            // 기존: 실제 카드 뽑기 + UI로 전달 (유지)
            PC->Client_ReceiveDealtCards(CardManager->DealCards(3));

            // [+] FX: 모션 3장
            if (ABombGameState* GS = GetGameState<ABombGameState>())
            {
                if (UBombCardManagerComponent* FX = GS->FindComponentByClass<UBombCardManagerComponent>())
                {
                    for (int32 i = 0; i < 3; ++i)
                    {
                        FX->Server_FlyCardToSeat(_curIndex, 0.45f);
                    }
                }
            }
        }
    }
}


void ABombGameMode::ContinueCurrentRound()
{
    CurrentRoundLogic();
}

void ABombGameMode::ExplodeBomb(const int32 damage)
{
    if (!HasAuthority()) return;

    // 폭발/종료 처리 중복 방지
    if (bResolvingRound) {
        UE_LOG(LogTemp, Warning, TEXT("ExplodeBomb skipped: already resolving"));
        return;
    }
    bResolvingRound = true;

    UE_LOG(LogTemp, Warning, TEXT("ExplodeBomb"));

    if (SpawnedBomb) { 
        SpawnedBomb->Destroy(); 
        SpawnedBomb = nullptr; 
    }
    if (CardManager) { 
        CardManager->ConditionalBeginDestroy(); 
        CardManager = nullptr; 
    }

    // HP 감소 (SetHp 내부에서 HP가 0이면 HandleCharacterDeath가 호출되어 Dead 애니메이션이 재생됨)
    if (PlayerControllers.IsValidIndex(_curIndex))
    {
        if (auto* PC = PlayerControllers[_curIndex])
        {
            if (auto* PS = Cast<ABombPlayerState>(PC->PlayerState))
            {
                const int32 NewHp = FMath::Clamp(PS->GetHp() - damage, 0, PS->GetMaxHp());
                PS->SetHp(NewHp);
                // SetHp 내부에서 HP가 0이 되면 HandleCharacterDeath()가 호출되어 Dead 상태가 설정됨
            }
        }
    }

    // Character -> 승리/패배 Anim (HP가 0이 아닌 경우에만)
    {
        const int32 n = PlayerControllers.Num();
        for (int32 i = 0; i < n; ++i)
        {
            if (ABombCharacter* C = GetCharacterAtSeat(i))
            {
                if (auto* PC = PlayerControllers[i])
                {
                    if (auto* PS = Cast<ABombPlayerState>(PC->PlayerState))
                    {
                        // HP가 0이 아닌 경우에만 Win/Lose 애니메이션 재생
                        if (PS->GetHp() > 0)
                        {
                            bool bDefeat = false;
                            if (i == _curIndex) {
                                bDefeat = true;
                            }
                            C->SetState_ServerAuth(bDefeat ? EPlayerAnimState::Lose : EPlayerAnimState::Win);
                        }
                        // HP가 0이면 이미 SetHp에서 Dead 상태가 설정됨
                    }
                }
            }
        }
    }

    // 생존 플레이어 수 체크
    const int32 AliveCount = CountAlivePlayers();
    if (AliveCount <= 1)
    {
        // 생존한 플레이어 찾기
        int32 WinnerSeatIndex = -1;
        for (int32 i = 0; i < PlayerControllers.Num(); ++i)
        {
            if (IsAliveAtIndex(i))
            {
                WinnerSeatIndex = i;
                break;
            }
        }
        
        // 게임 종료
        bGameEnded = true;
        
        // 모든 플레이어 컨트롤러에게 Client RPC로 Win/Lose 위젯 표시 알림
        for (int32 i = 0; i < PlayerControllers.Num(); ++i)
        {
            if (ABombPlayerController* PC = PlayerControllers[i])
            {
                // 각 클라이언트에게 승자 정보를 RPC로 전달
                PC->Client_ShowWinWidget(WinnerSeatIndex);
            }
        }
        
        // 타이머 클리어 (다음 라운드 시작하지 않음)
        GetWorld()->GetTimerManager().ClearTimer(EndRoundHandle);
        GetWorld()->GetTimerManager().ClearTimer(RoundInitHandle);
        
        return;
    }

    GetWorld()->GetTimerManager().ClearTimer(EndRoundHandle);
    GetWorld()->GetTimerManager().SetTimer(
        EndRoundHandle, this, &ABombGameMode::EndCurrentRound, EndRoundDelay, false
    );
}

void ABombGameMode::Red_VisibleRemainingCount(const int32 Count) {
    UE_LOG(LogTemp, Warning, TEXT("BombGameMode:: Red_VisibleRemainingCount"));
    if (PlayerControllers.IsValidIndex(_curIndex)) 
    { 
        if (ABombPlayerController* PC = Cast<ABombPlayerController>(PlayerControllers[_curIndex])) 
        { 
            PC->Client_VisibleRemainingCount(Count); 
        } 
    } 
}

void ABombGameMode::OnBombArrivedAtSeat(int32 SeatIndex)
{
    if (!IsAliveAtIndex(SeatIndex)) return;

    // 전원 Idle, 도착 좌석만 Wait
    PushStateToAll(EPlayerAnimState::Idle, SeatIndex);
}


// === Utility ===
int32 ABombGameMode::CountAlivePlayers() const
{
    int32 Alive = 0;
    const int32 N = PlayerControllers.Num();
    for (int32 i = 0; i < N; ++i)
    {
        if (auto* PC = PlayerControllers[i])
            if (auto* PS = Cast<ABombPlayerState>(PC->PlayerState))
                if (PS->GetIsAlive())
                    ++Alive;
    }
    return Alive;
}

bool ABombGameMode::IsAliveAtIndex(int32 index) const
{
    if (!PlayerControllers.IsValidIndex(index)) return false;
    if (auto* PC = PlayerControllers[index])
        if (auto* PS = Cast<ABombPlayerState>(PC->PlayerState))
            return PS->GetIsAlive();
    return false;
}

ABombCharacter* ABombGameMode::GetCharacterAtSeat(int32 Seat) const
{
    if (!PlayerControllers.IsValidIndex(Seat)) return nullptr;

    if (ABombPlayerController* PC = Cast<ABombPlayerController>(PlayerControllers[Seat]))
    {
        return Cast<ABombCharacter>(PC->GetCharacter());
    }
    return nullptr;
}


void ABombGameMode::PushStateToAll(EPlayerAnimState DefaultState, int32 Except)
{
    const int32 n = PlayerControllers.Num();
    for (int32 i = 0; i < n; ++i)
    {
        if (ABombCharacter* C = GetCharacterAtSeat(i))
        {
            bool bWait = false;
            if (i == Except) {
                bWait = true;
            }
            C->SetState_ServerAuth(bWait ? EPlayerAnimState::Wait : DefaultState);
        }
    }
}



bool ABombGameMode::AdvanceTurn(TArray<int32>& Path)
{
    Path.Reset();

    const int32 N = SpawnPoints.Num();
    if (N == 0) return false;

    NumOfPlayerInRound = CountAlivePlayers();
    if (NumOfPlayerInRound <= 1) return false;

    const int32 dir = _bInverseOrder ? -1 : 1;

    int32 Cur = _curIndex;
    bool bFound = false;

    for (int32 i = 0; i < N; ++i)
    {
        Cur = (Cur + dir + N) % N;
        Path.Add(Cur);
        if (IsAliveAtIndex(Cur))
        {
            bFound = true;
            break;
        }
    }

    if (!bFound) return false;

    _curIndex = Cur;
    return true;
}
