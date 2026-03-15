// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actor/BombCardActor.h"
#include "BombCardManagerComponent.generated.h"

/** 좌석 포인트 (단순히 도착지) */
USTRUCT(BlueprintType)
struct FSeatTarget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetPoint = nullptr; // 좌석 위치
};

/** 카드 이펙트 매니저 (딜 애니메이션만 담당) */
UCLASS(ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class SPINNINGBOMB_API UBombCardManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UBombCardManagerComponent();

    /** 카드 FX 액터(BP_ABombCardActor) */
    UPROPERTY(EditAnywhere, Category = "CardFX")
    TSubclassOf<ABombCardActor> CardActorClass;

    /** (옵션) 시작 지점을 액터로 지정하고 싶을 때 */
    UPROPERTY(EditAnywhere, Category = "CardFX")
    AActor* CenterPoint = nullptr;

    /** 시작 지점을 '위치값'으로 직접 지정하고 싶을 때 (에디터에서 핸들로 수정 가능) */
    UPROPERTY(EditAnywhere, Category = "CardFX", meta = (MakeEditWidget = "true"))
    FVector CenterLocation = FVector::ZeroVector;

    /** 좌석 인덱스 → 도착 지점 */
    UPROPERTY(EditAnywhere, Category = "CardFX")
    TMap<int32, FSeatTarget> SeatTargets;

    /** 서버: 중앙→특정 좌석 한 장 */
    UFUNCTION(Server, Reliable)
    void Server_FlyCardToSeat(int32 SeatIdx, float TravelTime = 2.45f);

    /** 서버: 중앙→모든 좌석 한 장씩 */
    UFUNCTION(Server, Reliable)
    void Server_FlyCardToAllSeats(float TravelTime = 0.45f);

    /** 폭탄 스폰 시점 등에서 코드로 중앙 위치를 지정하고 싶을 때 */
    UFUNCTION(BlueprintCallable, Category = "CardFX")
    void SetCenterLocation(const FVector& InCenter) { CenterLocation = InCenter; }

private:
    void SpawnAndLaunch(AActor* Target, float TravelTime);
    FTransform GetCenterSpawnTransform() const;
};