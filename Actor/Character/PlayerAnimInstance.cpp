// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/PlayerAnimInstance.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerState = EPlayerAnimState::Idle;
}
