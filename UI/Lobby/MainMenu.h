// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainMenu.generated.h"


class UButton;
/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

public:


protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* UserImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UserName;


	UPROPERTY(meta = (BindWidget))
	UButton* StartWithOnlineBtt;

	UPROPERTY(meta = (BindWidget))
	UButton* StartWithComputerBtt;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingBtt;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitBtt;


	UFUNCTION(BlueprintCallable, category = "Function")
	void OnClickedStartOnline();

private:


};
