// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChatSystem/ChatWidget.h"
#include "UI/ChatSystem/ChatMessageRow.h"

#include "PlayerController/BombPlayerController.h"

#include "Components/EditableText.h"
#include "Components/ScrollBox.h"

UChatWidget::UChatWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ChatMessageRowBP(TEXT("'/Game/UI/ChatSystem/WBP_ChatMessageRow.WBP_ChatMessageRow_C'"));
	if (!ensure(ChatMessageRowBP.Class != nullptr)) return;

	ChatMessageRow = ChatMessageRowBP.Class;
}

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ensure(ChatText != nullptr)) return;
	ChatText->OnTextCommitted.AddDynamic(this, &UChatWidget::OnTextCommitted);

	ChatText->SetIsEnabled(false);
}

void UChatWidget::AddChatMessage(const FString& Message)
{
	if (ChatMessageRow)
	{
		UChatMessageRow* ChatRow = CreateWidget<UChatMessageRow>(this, ChatMessageRow);
		ChatRow->SetChatMessage(Message);

		if (!ensure(ScrollBox != nullptr)) return;
		ScrollBox->AddChild(ChatRow);
		ScrollBox->ScrollToEnd();
		//ScrollBox->bAnimateWheelScrolling = true;

	}
}

void UChatWidget::ActivateChatUI()
{
	if (ChatText)
	{
		ChatText->SetIsEnabled(true);
		ChatText->SetFocus();
		UE_LOG(LogTemp, Warning, TEXT("ChatWidget: ActivateChatUI()"));
	}
}

void UChatWidget::OnTextCommitted(const FText& text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		FText InputText = ChatText->GetText();
		FString TrimmedText = InputText.ToString().TrimStartAndEnd();

		if (!TrimmedText.IsEmpty())
		{
			if(ABombPlayerController* Controller = Cast<ABombPlayerController>(GetOwningPlayer()))
			{
				Controller->Server_SendChatMessage(TrimmedText);

				FInputModeGameOnly InputMode;
				Controller->SetInputMode(InputMode);

				ChatText->SetText(FText::GetEmpty());
				ChatText->SetIsEnabled(false);
			}
		}
	}
}
