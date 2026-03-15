// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Bomb/Bomb.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Actor/Bomb/BombAnimInstance.h"	//Bomb Anim

#include "Object/Card/CardDataTable.h"

#include "TimerManager.h"
#include "Net/UnrealNetwork.h"	//DOREPLIFETIME
#include "Kismet/GameplayStatics.h"


ABomb::ABomb(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	//bReplicates = true;
	//SetReplicates(true);
	//SetReplicateMovement(true);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMeshComp;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BombSM(TEXT("'/Game/Asset/BombAnim/SM_bomb.SM_bomb''"));
	if (BombSM.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(BombSM.Object);
	}

}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();

	BombAnim = Cast<UBombAnimInstance>(SkeletalMeshComp->GetAnimInstance());
	EmissiveMaterial = SkeletalMeshComp->CreateAndSetMaterialInstanceDynamic(0);


	if (HasAuthority())
	{
		BombState = EBombState::Initial;
		FromSeat = -1;
		ToSeat = -1;

		bDetonated = false;
		bHandlingPath = false;
		PendingPath.Reset();
		PathCursor = 0;
		CurrentSeat = -1;
		Damage = FMath::RandRange(3, 5);
		Count = FMath::RandRange(5, 10);
		UE_LOG(LogTemp, Warning, TEXT("Bomb: Damage=%d, Count=%d"), Damage, Count);
	}

	PlayBombSound(EBombSound::Raised);
}

void ABomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABomb, BombState);
	DOREPLIFETIME(ABomb, FromSeat);
	DOREPLIFETIME(ABomb, ToSeat);
}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABomb::OnRaisedTable()
{
	BombState = EBombState::Raised;

	OnRep_BombState();

	//TODO

}

// 경로
void ABomb::MoveInNewRound(int32 PlayerId)
{
	if (!HasAuthority() || bDetonated) return;

	CurrentSeat = -1;          // 센터에서 출발
	PendingPath.Reset();
	PendingPath.Add(PlayerId);
	PathCursor = 0;
	bHandlingPath = true;

	if (BombState != EBombState::Raised) 
	{
		GetWorld()->GetTimerManager().SetTimer(InitialBombTimer, this, &ABomb::StartNextStep, 1.f);
	}
	else
	{
		StartNextStep();
	}
}

void ABomb::PlayNextPath(const TArray<int32>& Path)
{
	if (!HasAuthority() || bDetonated) return;
	if (Path.Num() == 0) return;

	PendingPath = Path;
	PathCursor = 0;
	bHandlingPath = true;

	StartNextStep();
}

void ABomb::StartNextStep()
{
	if (!HasAuthority() || bDetonated) return;

	if (!bHandlingPath || !PendingPath.IsValidIndex(PathCursor))
	{
		FinishStep();
		return;
	}

	PlayBombSound(EBombSound::MoveStart);

	const int32 From = CurrentSeat;
	const int32 To = PendingPath[PathCursor];

	FromSeat = From;
	ToSeat = To;

	UE_LOG(LogTemp, Warning, TEXT("Bomb:: Move From : %d, To: %d"), FromSeat, ToSeat);

	Multicast_PlayStep(From, To);
}



void ABomb::FinishStep()
{
	if (!HasAuthority() || bDetonated) return;
	if (!bHandlingPath) return;

	if (PendingPath.IsValidIndex(PathCursor))
	{
		CurrentSeat = PendingPath[PathCursor];
	}

	CurrentSeat = PendingPath[PathCursor];

	++PathCursor;

	PlayBombSound(EBombSound::MoveStop);

	if (PendingPath.IsValidIndex(PathCursor))
	{
		// 한 틱 쉬고 다음 스텝 
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ABomb::StartNextStep);
	}
	else
	{
		bHandlingPath = false;

		BombState = EBombState::Idle;
		OnRep_BombState();

		// 카운트 ↓ / 폭발
		if (--Count > 0)
		{
			if (auto* GM = GetGM())
			{
				GM->OnBombArrivedAtSeat(CurrentSeat);
				GM->DealCardToCurrentPlayer();
			}
		}
		else
		{
			OnTriggeredExplode();
		}
	}
}


void ABomb::OnTriggeredExplode()
{
	if (!HasAuthority() || bDetonated) return;
	bDetonated = true;

	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().ClearTimer(InitialBombTimer);
	GetWorld()->GetTimerManager().ClearTimer(EmissiveColorHandle);

	PlayBombSound(EBombSound::Explode);

	BombState = EBombState::Explode;
	OnRep_BombState();
}



void ABomb::ApplyCardEffect(const FName& CardKey)
{
	if (!HasAuthority() || bDetonated) return;

	const int32 Key = FCString::Atoi(*CardKey.ToString());
	const ECardColor Color = GetCardColor(Key);
	const int32 SubId = Key % 100;

	switch (Color)
	{
	case ECardColor::Red:    HandleRedCard(SubId);    break;
	case ECardColor::Blue:   HandleBlueCard(SubId);   break;
	case ECardColor::Yellow: HandleYellowCard(SubId); break;
	case ECardColor::Purple: HandlePurpleCard(SubId); break;
	default: break;
	}
}


void ABomb::GenerateFlameParticle()
{
	// TODO : Paricle System
	Multicast_ExplosionEffects();

	if (HasAuthority())
	{
		if (auto* GM = GetGM())
		{
			GM->ExplodeBomb(Damage);
		}
	}
}

void ABomb::OnRep_BombState()
{
	if (!BombAnim) 
	{
		BombAnim = Cast<UBombAnimInstance>(SkeletalMeshComp->GetAnimInstance());
	}
		
	if (BombAnim)
	{
		BombAnim->SetBombState(BombState);
	}
}

void ABomb::OnRep_FromToSeat()
{
	if (!BombAnim)
	{
		BombAnim = Cast<UBombAnimInstance>(SkeletalMeshComp->GetAnimInstance());
	}
	if (BombAnim)
	{
		BombAnim->SetFromToAnim(FromSeat, ToSeat);
	}
}

ECardColor ABomb::GetCardColor(int32 CardId)
{
	int32 Color = CardId / 100;
	switch (Color)
	{
	case 1: return ECardColor::Red;
	case 2: return ECardColor::Blue;
	case 3: return ECardColor::Yellow;
	case 4: return ECardColor::Purple;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Card Color Unknown"));
		return ECardColor::Red;
	}
}

void ABomb::HandleRedCard(int32 SubID)
{

	if (SubID < 6) {
		switch (SubID) {
		case 1: Damage = FMath::Clamp(Damage + 1, 1, 100); break;
		case 2: Damage = FMath::Clamp(Damage + 2, 1, 100); break;
		case 3: Damage = FMath::Clamp(Damage - 1, 1, 100); break;
		case 4: Damage = FMath::Clamp(Damage - 2, 1, 100); break;
		case 5: Damage = FMath::Clamp(Damage + FMath::RandRange(-5, 5), 1, 100); break;
		default: break;
		}
		Multicast_EmissiveBombColor(ECardColor::Red);
		return;
	}
	if (SubID < 10) {
		switch (SubID) {
		case 6: Count = FMath::Clamp(Count + 1, 1, 100); break;
		case 7: Count = FMath::Clamp(Count + 2, 1, 100); break;
		case 8: Count = FMath::Clamp(Count - 1, 1, 100); break;
		case 9: Count = FMath::Clamp(Count + FMath::RandRange(-3, 3), 1, 100); break;
		default: break;
		}
		Multicast_EmissiveBombColor(ECardColor::Red);
		return;
	}

	if (SubID == 10) {
		if (auto* GM = GetGM()) GM->Red_VisibleRemainingCount(Count);
		Multicast_EmissiveBombColor(ECardColor::Red);
	}
	else {
		Multicast_EmissiveBombColor(ECardColor::Yellow); // Bluffing placeholder
	}

}

void ABomb::HandleBlueCard(int32 SubID)
{
	if (auto* GM = GetGM())
	{
		switch (SubID)
		{
		case 1: GM->Effect_MakeInverseOrder();           break;
		case 2: GM->Effect_JumpNextPlayer();             break;
		case 3: GM->Effect_BlockEffect();                break;
		case 4: GM->Effect_EliminateYellowCards();       break;
		case 5: GM->Effect_MakeDealLessCardNextTurn();   break;
		case 6: /* Steal coin */                         break;
		default: break;
		}
		Multicast_EmissiveBombColor(ECardColor::Blue);
	}
}

void ABomb::HandleYellowCard(int32 SubID)
{
	switch (SubID)
	{
	case 1:
		// 301 Throw Bomb
		// GM ->
		break;

	case 2:
		// 302 Half Damage
		// GM -> 
		break;

	case 3:
		// 303 Die Together
		// GM-> 
		break;

	case 4:
		// 304 Make me die
		// GM ->
		break;

	case 5:
		// 305 change Damage to Coin
		// GM -> 
		break;

	case 6:
		// 306 
		// GM 
		break;

	default:
		break;
	}

	Multicast_EmissiveBombColor(ECardColor::Yellow);
	UE_LOG(LogTemp, Warning, TEXT("Bomb: Yellow Effect Is Activated"));
}

void ABomb::HandlePurpleCard(int32 SubID)
{

}


void ABomb::Multicast_PlayStep_Implementation(int32 From, int32 To)
{
	if (!BombAnim)
	{
		BombAnim = Cast<UBombAnimInstance>(SkeletalMeshComp->GetAnimInstance());
	}
	if (BombAnim)
	{
		BombAnim->SetFromToAnim(From, To);
		BombAnim->SetBombState(EBombState::Move);
	}
}

void ABomb::Multicast_EmissiveBombColor_Implementation(ECardColor Color)
{
	if (!EmissiveMaterial) return;

	FLinearColor Glow = FLinearColor::Black;
	if (Color == ECardColor::Red)    Glow = FLinearColor::Red;
	else if (Color == ECardColor::Blue)   Glow = FLinearColor::Blue;
	else if (Color == ECardColor::Yellow) Glow = FLinearColor::Yellow;

	EmissiveMaterial->SetVectorParameterValue("GlowColor", Glow);
	GetWorld()->GetTimerManager().SetTimer(EmissiveColorHandle, this, &ABomb::StopEmissive, 1.f, false);
}


void ABomb::StopEmissive()
{
	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().ClearTimer(EmissiveColorHandle);

	if (EmissiveMaterial)
		EmissiveMaterial->SetVectorParameterValue("GlowColor", FLinearColor::Black);

	if (!HasAuthority() || bDetonated) return;

	// 카드 효과 후 다음 턴 진행
	if (auto* GM = GetGM())
		GM->ContinueCurrentRound();
}

void ABomb::Multicast_ExplosionEffects_Implementation()
{
	if (ExplosionFX)
	{
		FVector Loc = GetActorLocation();
		FRotator Rot = GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, Loc, Rot, true);
	}

}

void ABomb::PlayBombSound(EBombSound SoundType)
{
	if (HasAuthority())
	{
		Multicast_PlayBombSound(SoundType);
	}
	else
	{
		Server_PlayBombSound(SoundType);
	}

}

void ABomb::Server_PlayBombSound_Implementation(EBombSound SoundType)
{
	Multicast_PlayBombSound(SoundType);
}

void ABomb::Multicast_PlayBombSound_Implementation(EBombSound SoundType)
{
	OnPlayBombSound(SoundType);
}
