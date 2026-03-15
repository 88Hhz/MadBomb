// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombCardActor.generated.h"


UCLASS()
class SPINNINGBOMB_API ABombCardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABombCardActor();

	virtual void Tick(float DeltaTime) override;

	/** 서버에서 호출 → 모든 클라에서 이동 시작 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BeginFly(AActor* Target, float TravelTime = 0.45f);
	void Multicast_BeginFly_Implementation(AActor* Target, float TravelTime);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

private:
	bool bFlying = false;
	float Elapsed = 0.f;
	float Duration = 0.45f;

	FVector StartLoc;
	FRotator StartRot;
	FVector TargetLoc;
	FRotator TargetRot;
};
