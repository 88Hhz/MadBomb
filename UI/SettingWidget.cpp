// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SettingWidget.h"

#include "GameFramework/GameUserSettings.h"
#include "Internationalization/Text.h"


#define LOCTEXT_NAMESPACE "SettingLabel"
static const TArray<FText> TextLabels =
{
	LOCTEXT("Low", "Low"),
	LOCTEXT("Medium", "Medium"),
	LOCTEXT("High", "High"),
	LOCTEXT("Epic", "Epic"),
	LOCTEXT("Cinematic", "Cinematic")
};
#undef LOCTEXT_NAMESPACE

static const TArray<int32> ResolutionWidths = { 1280, 1600, 1920, 2560, 3840 };


void USettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->LoadSettings(true);

		WindowMode = Settings->GetFullscreenMode();
		TextureIndex = Settings->GetTextureQuality();

		bIsVSync = Settings->IsVSyncEnabled();
		ShadowIndex = Settings->GetShadowQuality();
		AntiAliasingIndex = Settings->GetAntiAliasingQuality();
		Resolution = Settings->GetScreenResolution();

		ResolutionIndex = ResolutionWidths.IndexOfByKey(Resolution.X);
	}
}

void USettingWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void USettingWidget::ApplySettings()
{
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetFullscreenMode(WindowMode);
		Settings->SetTextureQuality(TextureIndex);
		Settings->SetVSyncEnabled(bIsVSync);
		Settings->SetShadowQuality(ShadowIndex);
		Settings->SetAntiAliasingQuality(AntiAliasingIndex);
		Settings->SetScreenResolution(Resolution);

		Settings->ApplySettings(false);
		Settings->SaveSettings();
	}
}

FText USettingWidget::GetWindowModueText() const
{
	FString str;
	if (WindowMode == EWindowMode::Fullscreen)
	{
		str = "FullScreen";
		return FText::FromString(str);
	}
	else if (WindowMode == EWindowMode::WindowedFullscreen)
	{
		str = "WindowedFullscreen";
		return FText::FromString(str);
	}
	else
	{
		str = "Windowed";
		return FText::FromString(str);
	}
}

FText USettingWidget::GetTextureText() const
{
	if (TextLabels.IsValidIndex(TextureIndex))
	{
		return TextLabels[TextureIndex];
	}
	int32 Clamp = FMath::Clamp(TextureIndex, 0, 4);
	return TextLabels[Clamp];
}

FText USettingWidget::GetResolutionText() const
{
	FString str = FString::Printf(TEXT("%d x %d"), Resolution.X, Resolution.Y);

	return FText::FromString(str);
}

FText USettingWidget::GetVSyncText() const
{
	if (bIsVSync)
	{
		return FText::FromString(TEXT("On"));
	}
	else
	{
		return FText::FromString(TEXT("Off"));
	}
}

FText USettingWidget::GetShadowText() const
{
	if (TextLabels.IsValidIndex(ShadowIndex))
	{
		return TextLabels[ShadowIndex];
	}
	int32 Clamp = FMath::Clamp(ShadowIndex, 0, 4);
	return TextLabels[Clamp];

}

FText USettingWidget::GetAntiAliasingText() const
{
	if (TextLabels.IsValidIndex(AntiAliasingIndex))
	{
		return TextLabels[AntiAliasingIndex];
	}
	int32 Clamp = FMath::Clamp(AntiAliasingIndex, 0, 4);
	return TextLabels[Clamp];
}


