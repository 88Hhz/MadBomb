// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BombHUD.generated.h"


class UDealLayerWidget;

UCLASS()
class SPINNINGBOMB_API ABombHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABombHUD();

protected:
	virtual void BeginPlay() override;

public:
	void ActivateChatUI();
	void AddChatMessage(const FString& Message);

	void UpdateMainWidget();

	void PlayDealAndSetCards(const TArray<FName>& DealtCards);
	void SetCardWidgetWithDealtCard(const TArray<FName>& DealtCards);
	void ClearCardWidget();

	void VisibleRemainingCount(const int32 Count);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> WBP_MainWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> WBP_ChatWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> WBP_CardWidget;


	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UDealLayerWidget> WBP_DealLayer;

	UPROPERTY()
	class UDealLayerWidget* DealLayer = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	class UTexture2D* CardBackTexture = nullptr;

	// 덱/타겟 위치(간단히 하드코딩; 반응형이면 계산함수로 대체)
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector2D DeckScreenPos = FVector2D(120.f, 80.f);

	UPROPERTY(EditAnywhere, Category = "UI")
	TArray<FVector2D> HandTargetPos = { FVector2D(900,850), FVector2D(1000,850), FVector2D(1100,850) };

private:
	UPROPERTY()
	class UBombMainWidget* MainWidget;

	UPROPERTY()
	class UChatWidget* ChatWidget;

	UPROPERTY()
	class UCardWidget* CardWidget;



	bool bAnnouncedReady = false;

	UFUNCTION()
	void TryReportClientReady(); // 한 번만 성공하도록 보장
};
