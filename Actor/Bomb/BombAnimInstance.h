// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Actor/Bomb/Bomb.h"

#include "BombAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UBombAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UBombAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, category = "Bomb")
	void SetBombState(EBombState NewState);

	UFUNCTION(BlueprintCallable, Category = "Bomb")
	void SetFromToAnim(int32 From, int32 To);

	UFUNCTION(BlueprintCallable, Category = "Bomb|Notify")
	void AnimNotify_BombStepFinished();

	UFUNCTION(BlueprintCallable, Category = "Bomb|Notify")
	void AnimNotify_BombDetonated();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
	EBombState BombState = EBombState::Initial;

	UPROPERTY(BlueprintReadOnly, Category = "Bomb|Move")
	int32 MoveCode = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Bomb|Move")
	int32 FromSeat = -1;

	UPROPERTY(BlueprintReadOnly, Category = "Bomb|Move")
	int32 ToSeat = -1;
private:



};
