// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/MainMenu.h"
#include "UI/HUD/LobbyHUD.h"

#include "BombGameInstance.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"


void UMainMenu::NativeOnInitialized()
{
	Super::OnInitialized();

	UBombGameInstance* GI = GetGameInstance<UBombGameInstance>();
	if (GI)
	{
		UserImage->SetBrushFromTexture(GI->GetSteamAvatarTexture());
		UserName->SetText(FText::FromString(GI->GetUserName()));
	}

	if (!ensure(StartWithOnlineBtt != nullptr)) return;
	StartWithOnlineBtt->OnClicked.AddDynamic(this, &UMainMenu::OnClickedStartOnline);


}

void UMainMenu::NativeConstruct()
{


}


void UMainMenu::OnClickedStartOnline()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ALobbyHUD* HUD = PC->GetHUD<ALobbyHUD>())
		{
			HUD->ShowServerMenu();
		}
	}
}
