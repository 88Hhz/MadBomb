// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/LobbyCharacter.h"
#include "BombGameInstance.h"

#include "BombPlayerState.h"
#include "UI/Lobby/LobbyPlayerListRow.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ALobbyCharacter::ALobbyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//bReplicates = true;
	//GetMesh()->SetIsReplicated(true);

	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (PlayerMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PlayerMesh.Object);
	}

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetWidgetSpace(EWidgetSpace::World);
	WidgetComp->SetupAttachment(GetMesh());
	WidgetComp->SetRelativeLocation(FVector(0.f, 50.f, 10.f));
	WidgetComp->SetDrawSize(FVector2D(200, 50));
	WidgetComp->SetWidgetClass(PlayerWidget);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

// Called when the game starts or when spawned
void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerWidget != nullptr)
	{
		ULobbyPlayerListRow* PlayerRow = Cast<ULobbyPlayerListRow>(WidgetComp->GetUserWidgetObject());
		if (!PlayerRow) return;

		PlayerRow->SetOwningCharacter(this);
	}
}

void ALobbyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}

void ALobbyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ULobbyPlayerListRow* PlayerRow = Cast<ULobbyPlayerListRow>(WidgetComp->GetUserWidgetObject());
	if (!PlayerRow) return;

	PlayerRow->UpdatePlayerName();

}

// Called every frame
void ALobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALobbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ALobbyCharacter::UpdateReadyState()
{
	if (WidgetComp)
	{
		ULobbyPlayerListRow* PlayerRow = Cast<ULobbyPlayerListRow>(WidgetComp->GetUserWidgetObject());
		if (!PlayerRow) return;

		PlayerRow->ReadyForGame();
	}
}
