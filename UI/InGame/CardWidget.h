// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardWidget.generated.h"

/**
 * 
 */
class UImage;
class UButton;
class UTextBlock;
class UDataTable;

enum class ECardEffect : uint8;
struct FCardData; 

UCLASS()
class SPINNINGBOMB_API UCardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCardWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable, category = "CardUI")
	void SetWithDealtCards(const TArray<FName>& DealtCards);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* AttackImage;

	UPROPERTY(meta = (BindWidget))
	UImage* AttackImage2;

	UPROPERTY(meta = (BindWidget))
	UImage* AttackImage3;

	UPROPERTY(meta = (BindWidget))
	UImage* DefenceImage;

	UPROPERTY(meta = (BindWidget))
	UImage* DefenceImage2;

	UPROPERTY(meta = (BindWidget))
	UImage* DefenceImage3;

	UPROPERTY(meta = (BindWidget))
	UImage* SpecialImage;

	UPROPERTY(meta = (BindWidget))
	UImage* SpecialImage2;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> CardDataTable;
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Card1 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Card2 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Card3 = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Name1 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Name2 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Name3 = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReDrawCardButton = nullptr;
public:
	// ȣ�� �� Ȯ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Hover")
	float HoverScale = 1.08f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Hover")
	float DefaultScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Hover")
	int32 HoverZOrder = 100;

private:
	void ApplyEffectToButton(UButton* Button, ECardEffect Effect, int32 CardID);
	const FSlateBrush* GetBrushByEffect(ECardEffect Effect, int32 CardID) const;
	void SetButtonBrushAllStates(UButton* Button, const FSlateBrush& Brush);


	void BindCardButtons();
	void UnbindCardButtons();
	void HandleCardClicked(int32 Index);

	UFUNCTION()
	void OnClickedSelectedCard1();

	UFUNCTION()
	void OnClickedSelectedCard2();

	UFUNCTION()
	void OnClickedSelectedCard3();

	UFUNCTION()
	void OnClickedReDrawCardButton();

	UFUNCTION()
	void OnHoveredSelectedCard1();
	UFUNCTION()
	void OnUnhoveredSelectedCard1();

	UFUNCTION()
	void OnHoveredSelectedCard2();
	UFUNCTION()
	void OnUnhoveredSelectedCard2();

	UFUNCTION()
	void OnHoveredSelectedCard3();
	UFUNCTION()
	void OnUnhoveredSelectedCard3();

	// ���� ��ƿ
	void SetWidgetScale(UWidget* W, float Scale);
	void BringToFront(UWidget* W, bool bFront);
private:
	TArray<FCardData> HandedCards;
	TArray<FName> CardKeys;

	TMap<TWeakObjectPtr<UWidget>, int32> OriginalZOrders;

	int32 Coin = -1;
};
