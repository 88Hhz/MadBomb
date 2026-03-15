// ABombCardActor.cpp
#include "Actor/BombCardActor.h"

ABombCardActor::ABombCardActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(false); // 위치는 클라 보간으로 처리

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABombCardActor::Multicast_BeginFly_Implementation(AActor* Target, float TravelTime)
{
	if (!Target) { Destroy(); return; }

	Duration = FMath::Max(0.05f, TravelTime);
	Elapsed = 0.f;
	bFlying = true;

	StartLoc = GetActorLocation();
	StartRot = GetActorRotation();
	TargetLoc = Target->GetActorLocation();
	TargetRot = Target->GetActorRotation();
}

void ABombCardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bFlying) return;

	Elapsed += DeltaTime;
	const float Alpha = FMath::Clamp(Elapsed / Duration, 0.f, 1.f);

	SetActorLocation(FMath::Lerp(StartLoc, TargetLoc, Alpha));
	SetActorRotation(FMath::Lerp(StartRot, TargetRot, Alpha));

	if (Alpha >= 1.f)
	{
		bFlying = false;
		Destroy(); // 도착하면 바로 제거
	}
}
