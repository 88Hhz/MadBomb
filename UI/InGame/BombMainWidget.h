// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BombMainWidget.generated.h"


class UImage;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UBombMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION()
	void UpdateCurrentRound();

	void UpdateHpWidget();
	void UpdateCoinWidget();
	
	void VisibleRemainingCount(const int32 Count);

protected:
	UPROPERTY(meta = (BindWidget)) UImage* PlayerAvatar = nullptr;
	UPROPERTY(meta = (BindWidget)) UTextBlock* PlayerName = nullptr;
	UPROPERTY(meta = (BindWidget)) UTextBlock* PlayerHp = nullptr;
	UPROPERTY(meta = (BindWidget)) UTextBlock* MatchRound = nullptr;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Countdown = nullptr;
	UPROPERTY(meta = (BindWidget)) UTextBlock* RemainCount = nullptr;
	UPROPERTY(meta = (BindWidget)) UProgressBar* ProgressBar_Hp = nullptr;

protected:
	int32 MaxHp = 0;
	int32 CurrentHp = 0;


private:
	void HiddenRemainCount();
	FTimerHandle VisibleCountHandle;

	float HpFillCurrent = 1.f;  // ProgressBar 현재 표시 (0~1)
	float HpFillTarget = 1.f;  // 목표 표시 (0~1)

};
