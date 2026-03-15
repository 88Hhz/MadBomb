// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UChatWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void AddChatMessage(const FString& Message);

	UFUNCTION()
	void ActivateChatUI();


protected:
	UPROPERTY(meta = (BindWidget))
	class UEditableText* ChatText;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox;

	UFUNCTION()
	void OnTextCommitted(const FText& text, ETextCommit::Type CommitMethod);

private:
	UPROPERTY()
	TSubclassOf<UUserWidget> ChatMessageRow;

};
