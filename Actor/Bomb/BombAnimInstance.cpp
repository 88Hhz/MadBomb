// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Bomb/BombAnimInstance.h"



UBombAnimInstance::UBombAnimInstance()
{

}

void UBombAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UBombAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}


void UBombAnimInstance::SetBombState(EBombState NewState)
{
	BombState = NewState;
}

void UBombAnimInstance::SetFromToAnim(int32 From, int32 To)
{
	FromSeat = From;
	ToSeat = To;

	const int32 from = (From < 0) ? 0 : From + 1;
	const int32 to = (To < 0) ? 0 : To + 1;
	MoveCode = from * 10 + to;

	UE_LOG(LogTemp, Warning, TEXT("BombAnim:: MoveCode: %d"), MoveCode);
}

void UBombAnimInstance::AnimNotify_BombStepFinished()
{
	if (ABomb* Bomb = Cast<ABomb>(GetOwningActor()))
	{
		if (Bomb->HasAuthority())
			Bomb->FinishStep();
	}
}

void UBombAnimInstance::AnimNotify_BombDetonated()
{
	//TODO: niagara
	if (ABomb* Bomb = Cast<ABomb>(GetOwningActor()))
	{
		Bomb->GenerateFlameParticle();
	}
}
