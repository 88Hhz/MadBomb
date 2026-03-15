// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateRow.generated.h"

class UTextBlock;
class UProgressBar;
class ACharacter;
class ABombPlayerState;


UCLASS()
class SPINNINGBOMB_API UPlayerStateRow : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeConstruct() override;

public:
	void SetOwningCharacter(ACharacter* NewChar);

	UFUNCTION(BlueprintCallable, category = "PlayerStateRow")
	void UpdateStateRow();


protected:

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, category = "PlayerStateRow")
	UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* PlayerHpBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerHp;

private:
	UPROPERTY()
	ACharacter* Char;

};
