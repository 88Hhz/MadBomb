// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessageRow.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UChatMessageRow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatMessage;
	
	void SetChatMessage(const FString& Message);
};
