// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/CardWidget.h"
#include "UI/HUD/BombHUD.h"
#include "PlayerController/BombPlayerController.h"
#include "BombPlayerState.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Object/Card/CardDataTable.h"

UCardWidget::UCardWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}


void UCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� �о�ͼ� ����ο� ��ư ���ü��� ����
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ABombPlayerState* PS = PC->GetPlayerState<ABombPlayerState>())
		{
			Coin = PS->GetCoin();
		}
	}

	if (ReDrawCardButton)
	{
		if (Coin < 1)
		{
			ReDrawCardButton->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ReDrawCardButton->SetVisibility(ESlateVisibility::Visible);
			ReDrawCardButton->OnClicked.AddDynamic(this, &UCardWidget::OnClickedReDrawCardButton);
		}
	}

	BindCardButtons();
}

void UCardWidget::NativeDestruct()
{
	UnbindCardButtons();
	Super::NativeDestruct();
}

void UCardWidget::SetWithDealtCards(const TArray<FName>& DealtCards)
{
	CardKeys = DealtCards;
	HandedCards.Reset();

	UDataTable* Table = LoadObject<UDataTable>(this, TEXT("DataTable'/Game/InGame/Card/DT_CardDataTable.DT_CardDataTable'"));
	if (!Table) return;

	static const FString context(TEXT("CardWidget::SetWithDealtCards"));

	// �ִ� 3�常 ó��
	const int32 Num = FMath::Min(DealtCards.Num(), 3);
	for (int32 i = 0; i < Num; ++i)
	{
		if (FCardData* Data = Table->FindRow<FCardData>(DealtCards[i], context, false))
		{
			HandedCards.Add(*Data);

			// �̸� ����
			if (i == 0 && Name1) Name1->SetText(FText::FromString(Data->CardName));
			if (i == 1 && Name2) Name2->SetText(FText::FromString(Data->CardName));
			if (i == 2 && Name3) Name3->SetText(FText::FromString(Data->CardName));

			// �̹��� ����(�ٽ�) : BP�� UImage Brush �� ��ư �� UImage�� ����
			if (i == 0) ApplyEffectToButton(Card1, Data->CardEffect, Data->CardID);
			if (i == 1) ApplyEffectToButton(Card2, Data->CardEffect, Data->CardID);
			if (i == 2) ApplyEffectToButton(Card3, Data->CardEffect, Data->CardID);
		}
	}

	UnbindCardButtons();
	BindCardButtons();
}



const FSlateBrush* UCardWidget::GetBrushByEffect(ECardEffect Effect, int32 CardID) const
{
	const int32 SubID = CardID % 100;

	switch (Effect)
	{
	case ECardEffect::Attack:
	{
		UImage* AttackImages[] = { AttackImage, AttackImage2, AttackImage3 };
		const int32 Index = FMath::Clamp((SubID - 1) % 3, 0, 2);
		if (UImage* SelectedImage = AttackImages[Index])
		{
			return &SelectedImage->GetBrush();
		}
		break;
	}
	case ECardEffect::Defence:
	{
		UImage* DefenceImages[] = { DefenceImage, DefenceImage2, DefenceImage3 };
		const int32 Index = FMath::Clamp((SubID - 1) % 3, 0, 2);
		if (UImage* SelectedImage = DefenceImages[Index])
		{
			return &SelectedImage->GetBrush();
		}
		break;
	}
	case ECardEffect::Special:
	{
		UImage* SpecialImages[] = { SpecialImage, SpecialImage2 };
		const int32 Index = FMath::Clamp((SubID - 1) % 2, 0, 1);
		if (UImage* SelectedImage = SpecialImages[Index])
		{
			return &SelectedImage->GetBrush();
		}
		break;
	}
	}

	return nullptr;
}

void UCardWidget::SetButtonBrushAllStates(UButton* Button, const FSlateBrush& Brush)
{
	if (!Button) return;
	FButtonStyle S = Button->GetStyle();
	S.Normal = Brush;
	S.Hovered = Brush;
	S.Pressed = Brush;
	Button->SetStyle(S);
}

void UCardWidget::ApplyEffectToButton(UButton* Button, ECardEffect Effect, int32 CardID)
{
	if (!Button) return;
	if (const FSlateBrush* Brush = GetBrushByEffect(Effect, CardID))
	{
		SetButtonBrushAllStates(Button, *Brush);
	}

	SetWidgetScale(Button, DefaultScale);
	BringToFront(Button, false);
}

void UCardWidget::BindCardButtons()
{
	if (Card1)
	{
		Card1->OnClicked.AddDynamic(this, &UCardWidget::OnClickedSelectedCard1);
		Card1->OnHovered.AddDynamic(this, &UCardWidget::OnHoveredSelectedCard1);
		Card1->OnUnhovered.AddDynamic(this, &UCardWidget::OnUnhoveredSelectedCard1);
	}

	if (Card2)
	{
		Card2->OnClicked.AddDynamic(this, &UCardWidget::OnClickedSelectedCard2);
		Card2->OnHovered.AddDynamic(this, &UCardWidget::OnHoveredSelectedCard2);
		Card2->OnUnhovered.AddDynamic(this, &UCardWidget::OnUnhoveredSelectedCard2);
	}

	if (Card3)
	{
		Card3->OnClicked.AddDynamic(this, &UCardWidget::OnClickedSelectedCard3);
		Card3->OnHovered.AddDynamic(this, &UCardWidget::OnHoveredSelectedCard3);
		Card3->OnUnhovered.AddDynamic(this, &UCardWidget::OnUnhoveredSelectedCard3);
	}
}

void UCardWidget::UnbindCardButtons()
{
	if (Card1)
	{
		Card1->OnClicked.RemoveDynamic(this, &UCardWidget::OnClickedSelectedCard1);
		Card1->OnHovered.RemoveDynamic(this, &UCardWidget::OnHoveredSelectedCard1);
		Card1->OnUnhovered.RemoveDynamic(this, &UCardWidget::OnUnhoveredSelectedCard1);
	}
	if (Card2)
	{
		Card2->OnClicked.RemoveDynamic(this, &UCardWidget::OnClickedSelectedCard2);
		Card2->OnHovered.RemoveDynamic(this, &UCardWidget::OnHoveredSelectedCard2);
		Card2->OnUnhovered.RemoveDynamic(this, &UCardWidget::OnUnhoveredSelectedCard2);
	}
	if (Card3)
	{
		Card3->OnClicked.RemoveDynamic(this, &UCardWidget::OnClickedSelectedCard3);
		Card3->OnHovered.RemoveDynamic(this, &UCardWidget::OnHoveredSelectedCard3);
		Card3->OnUnhovered.RemoveDynamic(this, &UCardWidget::OnUnhoveredSelectedCard3);
	}
}

void UCardWidget::HandleCardClicked(int32 Index)
{
	if (!CardKeys.IsValidIndex(Index)) return;

	if (ABombPlayerController* PC = GetOwningPlayer<ABombPlayerController>())
	{
		PC->Server_SendSelectedCard(CardKeys[Index]);
	}

	UnbindCardButtons();

	// HUD���� �ڱ� �ڽ� ���� (���� ���� ����)
	if (ABombPlayerController* PC = GetOwningPlayer<ABombPlayerController>())
	{
		if (ABombHUD* HUD = PC->GetHUD<ABombHUD>())
		{
			HUD->ClearCardWidget();
		}
	}
}

void UCardWidget::OnClickedSelectedCard1() { HandleCardClicked(0); }
void UCardWidget::OnClickedSelectedCard2() { HandleCardClicked(1); }
void UCardWidget::OnClickedSelectedCard3() { HandleCardClicked(2); }


void UCardWidget::OnClickedReDrawCardButton()
{
	UnbindCardButtons();

	if (--Coin < 1 && ReDrawCardButton)
	{
		ReDrawCardButton->SetVisibility(ESlateVisibility::Hidden);
		ReDrawCardButton->OnClicked.Clear();
	}

	if (ABombPlayerController* PC = GetOwningPlayer<ABombPlayerController>())
	{
		PC->Server_ReDrawCards();
	}
}


// ===== Hover Handlers =====
void UCardWidget::OnHoveredSelectedCard1()
{
	BringToFront(Card1, true);
	SetWidgetScale(Card1, HoverScale);
}
void UCardWidget::OnUnhoveredSelectedCard1()
{
	SetWidgetScale(Card1, DefaultScale);
	BringToFront(Card1, false);
}

void UCardWidget::OnHoveredSelectedCard2()
{
	BringToFront(Card2, true);
	SetWidgetScale(Card2, HoverScale);
}
void UCardWidget::OnUnhoveredSelectedCard2()
{
	SetWidgetScale(Card2, DefaultScale);
	BringToFront(Card2, false);
}

void UCardWidget::OnHoveredSelectedCard3()
{
	BringToFront(Card3, true);
	SetWidgetScale(Card3, HoverScale);
}
void UCardWidget::OnUnhoveredSelectedCard3()
{
	SetWidgetScale(Card3, DefaultScale);
	BringToFront(Card3, false);
}

// ===== Common Utils =====
void UCardWidget::SetWidgetScale(UWidget* W, float Scale)
{
	if (!W) return;
	FWidgetTransform T = W->GetRenderTransform();
	T.Scale = FVector2D(Scale, Scale);
	W->SetRenderTransform(T);

}

void UCardWidget::BringToFront(UWidget* W, bool bFront)
{
	if (!W) return;

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(W->Slot))
	{
		if (bFront)
		{
			if (!OriginalZOrders.Contains(W))
			{
				OriginalZOrders.Add(W, CanvasSlot->GetZOrder());
			}
			CanvasSlot->SetZOrder(HoverZOrder);
		}
		else
		{
			int32* Prev = OriginalZOrders.Find(W);
			CanvasSlot->SetZOrder(Prev ? *Prev : 0);
			OriginalZOrders.Remove(W);
		}
	}
}