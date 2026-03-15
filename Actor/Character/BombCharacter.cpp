// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Character/BombCharacter.h"

#include "UI/InGame/PlayerStateRow.h"
#include "Actor/Character/PlayerAnimInstance.h"
#include "BombPlayerState.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

ABombCharacter::ABombCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    bReplicates = true;
    if (USkeletalMeshComponent* SM = GetMesh())
    {
        SM->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
        //SM->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
        SM->bOnlyAllowAutonomousTickPose = false;
        SM->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    }

    WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
    WidgetComp->SetupAttachment(GetMesh());
    WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComp->SetRelativeLocation(FVector(0.f, 50.f, 10.f));
    WidgetComp->SetDrawSize(FVector2D(200.f, 70.f));
    WidgetComp->SetDrawAtDesiredSize(false);
}

void ABombCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (WidgetComp && PlayerWidget)
    {
        WidgetComp->SetWidgetClass(PlayerWidget);
    }

    if (UPlayerStateRow* Row = GetPlayerStateRow())
    {
        Row->SetOwningCharacter(this);
        UpdateHpWidget();
    }

    // AnimInstance�� �غ�� �� ���� ���� �� �� �о��ֱ�
    ApplyAnimState();
    
    // Win/Lose 위젯 초기화 (로컬 플레이어만)
    if (IsLocallyControlled())
    {
        if (APlayerController* PC = GetController<APlayerController>())
        {
            // Win 위젯 초기화
            if (WinWidgetClass)
            {
                WinWidget = CreateWidget<UUserWidget>(PC, WinWidgetClass);
                if (WinWidget)
                {
                    WinWidget->AddToViewport(10); // 높은 Z-order로 표시
                    WinWidget->SetVisibility(ESlateVisibility::Collapsed); // 처음에는 숨김
                }
            }
            
            // Lose 위젯 초기화
            if (LoseWidgetClass)
            {
                LoseWidget = CreateWidget<UUserWidget>(PC, LoseWidgetClass);
                if (LoseWidget)
                {
                    LoseWidget->AddToViewport(10); // 높은 Z-order로 표시
                    LoseWidget->SetVisibility(ESlateVisibility::Collapsed); // 처음에는 숨김
                }
            }
        }
    }
}

void ABombCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABombCharacter, AnimState);
}

void ABombCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    UpdateHpWidget();
}

void ABombCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABombCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABombCharacter::SetState_ServerAuth(EPlayerAnimState NewState)
{
    if (!HasAuthority()) return;
    if (AnimState == NewState) return;

    AnimState = NewState;
    ApplyAnimState();
    Multicast_PushState(NewState);
}

void ABombCharacter::ApplyAnimState()
{
    if (UPlayerAnimInstance* PAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance()))
    {
        PAnim->SetState(AnimState);
    }
}

void ABombCharacter::UpdateHpWidget()
{
    if (UPlayerStateRow* Row = GetPlayerStateRow())
    {
        Row->UpdateStateRow();
    }
}

void ABombCharacter::OnRep_State()
{
    ApplyAnimState();
}

UPlayerStateRow* ABombCharacter::GetPlayerStateRow() const
{
    if (!WidgetComp) return nullptr;
    if (UUserWidget* UW = WidgetComp->GetUserWidgetObject())
    {
        return Cast<UPlayerStateRow>(UW);
    }
    return nullptr;
}

void ABombCharacter::Multicast_PushState_Implementation(EPlayerAnimState NewState)
{
    AnimState = NewState;
    ApplyAnimState();
}

void ABombCharacter::ShowWinWidget(int32 WinnerSeatIndex)
{
    // 자신이 로컬 플레이어인지 확인
    if (!IsLocallyControlled()) return;
    
    // 자신의 Seat Index 가져오기
    int32 MySeatIndex = -1;
    if (ABombPlayerState* PS = GetPlayerState<ABombPlayerState>())
    {
        MySeatIndex = PS->GetPlayerTurnOrder();
    }
    
    // 승자인지 확인
    bool bIsWinner = (MySeatIndex == WinnerSeatIndex);
    
    if (bIsWinner)
    {
        // Win 위젯 표시, Lose 위젯 숨김
        if (WinWidget)
        {
            WinWidget->SetVisibility(ESlateVisibility::Visible);
        }
        if (LoseWidget)
        {
            LoseWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
        // 블루프린트 이벤트 호출 (Win 위젯 표시)
        OnShowWinWidget(WinnerSeatIndex);
    }
    else
    {
        // Lose 위젯 표시, Win 위젯 숨김
        if (LoseWidget)
        {
            LoseWidget->SetVisibility(ESlateVisibility::Visible);
        }
        if (WinWidget)
        {
            WinWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
        // 블루프린트 이벤트 호출 (Lose 위젯 표시)
        OnShowLoseWidget(WinnerSeatIndex);
    }
}
