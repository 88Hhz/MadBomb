// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Card/CardDataTable.h"

#include "UObject/NoExportTypes.h"
#include "Card.generated.h"



/**
 * 
 */
UCLASS(Blueprintable)
class SPINNINGBOMB_API UCard : public UObject
{
	GENERATED_BODY()
	
public:
	UCard(const FObjectInitializer& ObjectInitializer);


protected:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Card")
	FCardData CardData;


public:
	UFUNCTION(BlueprintCallable)
	void InitializeCard(const FCardData& Data) 
	{
		CardData = Data;
	}

protected:




private:


};
