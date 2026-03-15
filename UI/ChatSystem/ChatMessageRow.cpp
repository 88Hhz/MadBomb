// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChatSystem/ChatMessageRow.h"

#include "Components/TextBlock.h"

void UChatMessageRow::SetChatMessage(const FString& Message)
{
	if (ChatMessage)
	{
		ChatMessage->SetText(FText::FromString(Message));
		//ChatMessage->Font.Size = 16;
	}
}
