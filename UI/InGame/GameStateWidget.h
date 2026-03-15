// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UGameStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void WaitforGame();
	void StartGame();


protected:


private:

};
