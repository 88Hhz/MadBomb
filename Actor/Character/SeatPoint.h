// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "SeatPoint.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ASeatPoint : public ATargetPoint
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Seat")
	int32 SeatIndex = 0; 

};
