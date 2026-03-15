// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Card/CardManager.h"
#include "Object/Card/Card.h"


UCardManager::UCardManager(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{


}


void UCardManager::InitializeCardPool()
{
	UE_LOG(LogTemp, Error, TEXT("CardManager: IntializeCardPool() Called"));
	UDataTable* CardDataTable = LoadObject<UDataTable>(this, TEXT("DataTable'/Game/InGame/Card/DT_CardDataTable.DT_CardDataTable'"));
	if (CardDataTable)
	{
		//Card Weight
		TMap<ECardType, int32> CardWeightMap;
		CardWeightMap.Add(ECardType::Red, 5);
		CardWeightMap.Add(ECardType::Blue, 3);
		CardWeightMap.Add(ECardType::Yellow, 2);
		CardWeightMap.Add(ECardType::Purple, 1);


		TArray<FName> RowNames = CardDataTable->GetRowNames();
		static const FString ContextString(TEXT("DataContext"));

		for (const FName& RowName : RowNames)
		{
			FCardData* CardData = CardDataTable->FindRow<FCardData>(RowName, ContextString, true);
			if (CardData)
			{
				int32* CardWeight = CardWeightMap.Find(CardData->CardType);
				if (CardWeight)
				{
					int32 Weight = *CardWeight;
					for (int32 i = 0; i < Weight; ++i)
					{
						CardPool.Add(RowName);
					}
				}
			}
		}

		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, FString::Printf(TEXT("CardMAnager::Initialize CardPool has %d . "), CardPool.Num()));
			UE_LOG(LogTemp, Warning, TEXT("CArdManager: Card Pool has %d"), CardPool.Num());
		}

		ShuffleCardPool();
	}
}


//Fisher-Yates Shuffle
void UCardManager::ShuffleCardPool()
{
	for (int32 i = 0; i < CardPool.Num(); ++i) {
		int32 Index = FMath::RandRange(0, CardPool.Num() - 1);
		CardPool.Swap(i, Index);
	}

	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, FString::Printf(TEXT("CardMAnager::ShuffleCardFin. ")));
	}
}



TArray<FName> UCardManager::DealCards(int32 NumCards)
{
	TArray<FName> DealtCards;
	for (int32 i = 0; i < NumCards; ++i)
	{
		if (CardPool.Num() <= 0) break;	//���� üũ

		FName CardKey = CardPool[0];
		DealtCards.Add(CardKey);

		UE_LOG(LogTemp, Warning, TEXT("CardManager::DealCArds add: %s"), *CardKey.ToString());

		CardPool.RemoveAt(0);
	}
	UE_LOG(LogTemp, Warning, TEXT("CardManager::DealCArds : %d"), DealtCards.Num());

	return DealtCards;
}
