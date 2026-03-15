// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Card/CardDataTable.h"

#include "UObject/NoExportTypes.h"
#include "CardManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SPINNINGBOMB_API UCardManager : public UObject
{
	GENERATED_BODY()
	

public:
	UCardManager(const FObjectInitializer& ObjectInitializer);
	

protected:


public:
	UFUNCTION(BlueprintCallable, category = "CardManager")
	void InitializeCardPool();

	UFUNCTION(BlueprintCallable, category = "CardManager")
	void ShuffleCardPool();

	UFUNCTION(BlueprintCallable, category = "CardManager")
	TArray<FName> DealCards(int32 NumCards = 3);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> CardPool;

private:


};
