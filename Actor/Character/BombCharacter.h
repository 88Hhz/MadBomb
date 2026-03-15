// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actor/Character/PlayerAnimInstance.h"
#include "BombCharacter.generated.h"

class UWidgetComponent;
class UUserWidget;
class UPlayerStateRow;

UCLASS()
class SPINNINGBOMB_API ABombCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABombCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_PlayerState() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetState_ServerAuth(EPlayerAnimState NewState);
	void ApplyAnimState();

	UFUNCTION(BlueprintCallable)
	void UpdateHpWidget();

	// Win 위젯 표시
	UFUNCTION(BlueprintCallable, Category = "Game")
	void ShowWinWidget(int32 WinnerSeatIndex);
	
	// 블루프린트에서 구현할 이벤트 (Win 위젯 표시)
	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void OnShowWinWidget(int32 WinnerSeatIndex);
	
	// 블루프린트에서 구현할 이벤트 (Lose 위젯 표시)
	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void OnShowLoseWidget(int32 WinnerSeatIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerUI")
	UWidgetComponent* WidgetComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerUI")
	TSubclassOf<UUserWidget> PlayerWidget;

	// Win 위젯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerUI")
	TSubclassOf<UUserWidget> WinWidgetClass;
	
	UPROPERTY()
	UUserWidget* WinWidget = nullptr;

	// Lose 위젯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerUI")
	TSubclassOf<UUserWidget> LoseWidgetClass;
	
	UPROPERTY()
	UUserWidget* LoseWidget = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_State, BlueprintReadOnly, Category = "State")
	EPlayerAnimState AnimState = EPlayerAnimState::Idle;

	UFUNCTION()
	void OnRep_State(); // Ŭ�󿡼� AnimInstance �ݿ�
private:
	UPlayerStateRow* GetPlayerStateRow() const;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PushState(EPlayerAnimState NewState);

};
