// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameMode/BombGameMode.h"
#include "Bomb.generated.h"


UENUM(BlueprintType)
enum class EBombState : uint8 {
	Initial,
	Raised,
	Move,
	Idle,
	Explode
};

UENUM(BlueprintType)
enum class ECardColor : uint8 {
	Red    UMETA(DisplayName = "Red"),   // 100번대
	Blue   UMETA(DisplayName = "Blue"),  // 200번대
	Yellow UMETA(DisplayName = "Yellow"), // 300번대
	Purple UMETA(DisplayName = "Purple"), // 300번대
};

UENUM(BlueprintType)
enum class EBombSound : uint8
{
	MoveStart  UMETA(DisplayName = "MoveStart"),
	MoveStop   UMETA(DisplayName = "MoveStop"),
	Explode    UMETA(DisplayName = "Explode"),
	Raised     UMETA(DisplayName = "Raised")
};

class UParticleSystem;


UCLASS()
class SPINNINGBOMB_API ABomb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABomb(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;



public:
	UFUNCTION(BlueprintCallable, category = "BombFunction")
	void OnRaisedTable();

	// 이동 단계 설정
	void MoveInNewRound(int32 PlayerId);
	void PlayNextPath(const TArray<int32>& Path);

	// 이동 단계 시작
	void StartNextStep();

	// 이동 단계 종료 후, 최종 도착 여부 판단
	void FinishStep();

	void OnTriggeredExplode();
	void ApplyCardEffect(const FName& CardKey);

public:
	UFUNCTION(BlueprintCallable, category = "BombState")
	void SetBombState(EBombState state) { BombState = state; }

	UFUNCTION(BlueprintCallable, category = "BombState")
	EBombState GetBombState() const { return BombState; }

	void GenerateFlameParticle();

protected:

	UPROPERTY(ReplicatedUsing = OnRep_BombState, BlueprintReadWrite, category = "BombState")
	EBombState BombState = EBombState::Initial;

	UFUNCTION()
	void OnRep_BombState();

	// 상태 복제 외에 From/To 복제 추가
	UPROPERTY(ReplicatedUsing = OnRep_FromToSeat, BlueprintReadOnly, Category = "BombState")
	int32 FromSeat = -1;

	UPROPERTY(ReplicatedUsing = OnRep_FromToSeat, BlueprintReadOnly, Category = "BombState")
	int32 ToSeat = -1;

	UFUNCTION()
	void OnRep_FromToSeat();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayStep(int32 From, int32 To);

	static ECardColor GetCardColor(int32 CardId);
	void HandleRedCard(int32 SubID);
	void HandleBlueCard(int32 SubID); 
	void HandleYellowCard(int32 SubID);
	void HandlePurpleCard(int32 SubID);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EmissiveBombColor(ECardColor Color);

	void StopEmissive();

	

protected:
	UPROPERTY(BlueprintReadWrite, category = "AnimInstance")
	class UBombAnimInstance* BombAnim;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp;

	UMaterialInstanceDynamic* EmissiveMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* ExplosionFX;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ExplosionEffects();
	void Multicast_ExplosionEffects_Implementation();

	/**Sound Effects*/
	UFUNCTION(BlueprintCallable, Category = "Bomb|SFX")
	void PlayBombSound(EBombSound SoundType);

	/** BP에서 구현: 여기서 Switch로 분기해서 PlaySound2D 해주면 됨 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Bomb|SFX")
	void OnPlayBombSound(EBombSound SoundType);

	UFUNCTION(Server, Reliable)
	void Server_PlayBombSound(EBombSound SoundType);

	/** 서버→모든 클라 (간단화를 위해 Reliable 하나로 통일) */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayBombSound(EBombSound SoundType);

private:
	bool bDetonated = false;

	int32 Count = 0;
	int32 Damage = 0;

	int32 CurrentSeat = -1;
	TArray<int32> PendingPath;
	int32 PathCursor = 0;
	bool bHandlingPath = false;

	FTimerHandle InitialBombTimer;
	FTimerHandle EmissiveColorHandle;

	class ABombGameMode* GetGM() const { 
		return GetWorld()->GetAuthGameMode<ABombGameMode>();
	};
};
