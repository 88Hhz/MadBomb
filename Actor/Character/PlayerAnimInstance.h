// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EPlayerAnimState : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Wait	UMETA(DisplayName = "Wait"),
	Win		UMETA(DisplayName = "Win"),
	Lose	UMETA(DisplayName = "Lose"),
	Dead	UMETA(DisplayName = "Dead")
};


UCLASS()
class SPINNINGBOMB_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	// AnimBP 상태머신 전이조건에 바로 쓰는 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EPlayerAnimState PlayerState = EPlayerAnimState::Idle;

	// C++/BP 어디서든 상태 바꾸기용
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetState(EPlayerAnimState InState) { PlayerState = InState; }

protected:
	virtual void NativeInitializeAnimation() override;


};
