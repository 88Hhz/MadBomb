// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()
public:


protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
public:

protected:
	UFUNCTION(BlueprintCallable, category = "Setting Function")
	void ApplySettings();

	UFUNCTION(BlueprintPure, category = "SettingText")
	FText GetWindowModueText() const;

	UFUNCTION(BlueprintPure, category = "SettingText")
	FText GetTextureText() const;

	UFUNCTION(BlueprintPure, category = "SettingText")
	FText GetResolutionText() const;

	UFUNCTION(BlueprintPure, category = "SettingText")
	FText GetVSyncText() const;

	UFUNCTION(BlueprintPure, category = "SettingText")
	FText GetShadowText() const;

	UFUNCTION(BlueprintPure, category = "SettingText")
	FText GetAntiAliasingText() const;

	

	UPROPERTY(BlueprintReadWrite, category = "Setting Variables")
	TEnumAsByte<EWindowMode::Type> WindowMode;

	UPROPERTY(BlueprintReadWrite, category = "Setting Variables")
	int32 TextureIndex = 0;

	UPROPERTY(BlueprintReadWrite, category = "Setting Variables")
	bool bIsVSync = false;

	UPROPERTY(BlueprintReadWrite, category = "Setting Variables")
	int32 ShadowIndex = 0;

	UPROPERTY(BlueprintReadWrite, category = "Setting Variables")
	int32 AntiAliasingIndex = 0;

	UPROPERTY(BlueprintReadWrite, category = "Setting Variables")
	FIntPoint Resolution;

	UPROPERTY(BlueprintReadWrite, category = "Setting Variables")
	int32 ResolutionIndex = 0;

private:
	
};