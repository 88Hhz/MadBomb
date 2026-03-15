// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/PlayerStateRow.h"

#include "BombPlayerState.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "GameFramework/Character.h"



void UPlayerStateRow::NativeConstruct()
{
	Super::NativeConstruct();

	if (Char)
	{
		if (ABombPlayerState* PS = Cast<ABombPlayerState>(Char->GetPlayerState()))
		{
			PlayerName->SetText(FText::FromString(PS->GetPlayerName()));
		}
	}
}

void UPlayerStateRow::SetOwningCharacter(ACharacter* NewChar)
{
	Char = NewChar;

	if (Char)
	{
		if (ABombPlayerState* PS = Cast<ABombPlayerState>(Char->GetPlayerState()))
		{
			if (PlayerName)
			{
				PlayerName->SetText(FText::FromString(PS->GetPlayerName()));
			}
		}
	}
}

void UPlayerStateRow::UpdateStateRow()
{
    if (!Char) return;

    if (ABombPlayerState* PS = Cast<ABombPlayerState>(Char->GetPlayerState()))
    {
        if (PlayerName) 
            PlayerName->SetText(FText::FromString(PS->GetPlayerName()));

        if (PlayerHpBar)
        {
            const float Percent = PS->GetHp() / (PS->GetMaxHp() * 1.f);
            PlayerHpBar->SetPercent(Percent);
        }

        if (PlayerHp)
            PlayerHp->SetText(FText::AsNumber(PS->GetHp()));
    }

}



