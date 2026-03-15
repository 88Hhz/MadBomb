// UBombCardManagerComponent.cpp
#include "Actor/BombCardManagerComponent.h"
#include "Engine/World.h"

UBombCardManagerComponent::UBombCardManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicated(false);
}

void UBombCardManagerComponent::Server_FlyCardToSeat_Implementation(int32 SeatIdx, float TravelTime)
{
    if (!GetOwner() || !GetOwner()->HasAuthority()) return;
    if (!CardActorClass) return;

    if (const FSeatTarget* Seat = SeatTargets.Find(SeatIdx))
    {
        if (Seat->TargetPoint)
        {
            SpawnAndLaunch(Seat->TargetPoint, TravelTime);
        }
    }
}

void UBombCardManagerComponent::Server_FlyCardToAllSeats_Implementation(float TravelTime)
{
    if (!GetOwner() || !GetOwner()->HasAuthority()) return;
    if (!CardActorClass) return;

    for (const auto& Pair : SeatTargets)
    {
        if (Pair.Value.TargetPoint)
        {
            SpawnAndLaunch(Pair.Value.TargetPoint, TravelTime);
        }
    }
}

FTransform UBombCardManagerComponent::GetCenterSpawnTransform() const
{
    if (CenterPoint)
        return CenterPoint->GetActorTransform();

    return FTransform(FRotator::ZeroRotator, CenterLocation, FVector::OneVector);
}

void UBombCardManagerComponent::SpawnAndLaunch(AActor* Target, float TravelTime)
{
    if (!Target || !CardActorClass) return;

    const FTransform SpawnTr = GetCenterSpawnTransform();

    FActorSpawnParameters SP;
    SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ABombCardActor* CardFx = GetWorld()->SpawnActor<ABombCardActor>(CardActorClass, SpawnTr, SP);
    if (CardFx)
    {
        CardFx->Multicast_BeginFly(Target, TravelTime);
    }
}