// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SessionDataStruct.generated.h"


USTRUCT(Atomic, BlueprintType)
struct FSessionDataStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString LobbyName;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFriendsOnly;

	UPROPERTY(BlueprintReadOnly)
	FString HostUserName;
};
/**
 * 
 */
class SPINNINGBOMB_API SessionDataStruct
{
public:
	SessionDataStruct();
	~SessionDataStruct();
};
