// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Sanctuary/EKSanctuary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EK_EnemyBase.h"
#include "Enemy/EK_EnemyStatusComponent.h"
#include "Subsystems/SanctuarySubsystem.h"
#include "UI/UISubsystem.h"
#include "EKGameplayTags.h"
#include "Blueprint/UserWidget.h"
#include "Player/EKPlayer/EKPlayer.h"

// Sets default values
AEKSanctuary::AEKSanctuary()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SMComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Niagara_Santuary = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara_Santuary->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AEKSanctuary::BeginPlay()
{
	Super::BeginPlay();

	USanctuarySubsystem* sanctuarySystem = GetGameInstance()->GetSubsystem<USanctuarySubsystem>();
	if (!sanctuarySystem) return;
	bActivated = sanctuarySystem->IsActivated(SanctuaryID);
	if (bActivated)
	{
		ActivateSantuary();
	}

	SaveMap();
}

// Called every frame
void AEKSanctuary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEKSanctuary::Interact()
{
	// Can't Interact While BossBattle
	UUISubsystem* UISystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
	if (!UISystem
		|| UISystem->GetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_Game_BossBattle) == ESlateVisibility::SelfHitTestInvisible)
		return;

	USanctuarySubsystem* sanctuarySystem = GetGameInstance()->GetSubsystem<USanctuarySubsystem>();
	if (!sanctuarySystem) return;

	if (!bActivated)
	{
		ActivateSantuary();

		sanctuarySystem->ActivateSanctuary(SanctuaryID);

		for (int i = 0; i < ShouldOpen_SanctuaryIDs.Num(); i++)
		{
			sanctuarySystem->ActivateSanctuary(ShouldOpen_SanctuaryIDs[i]);
		}

		bActivated = true;
	}

	sanctuarySystem->VisitSanctuary(SanctuaryID);

	UISystem->SetLayerVisibility(FEKGameplayTags::Get().UI_Layer_GameMenu, ESlateVisibility::SelfHitTestInvisible);
	UISystem->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_Santuary, ESlateVisibility::SelfHitTestInvisible);

	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	if (pc)
	{
		FInputModeUIOnly UIInputMode;
		pc->SetInputMode(UIInputMode);
		pc->SetShowMouseCursor(true);
	}

	ACharacter* character = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!character) return;
	AEKPlayer* player = Cast<AEKPlayer>(character);
	if (!player) return;
	player->PlayerRestore();
}

void AEKSanctuary::ActivateSantuary()
{
	GetWorld()->GetTimerManager().SetTimer(ActivateHandle,
		[&]() {
			CurrentScale = CurrentScale + FVector(0.05, 0.05, 0.05);
			Niagara_Santuary->SetWorldScale3D(CurrentScale);
		},
		0.02, true);

	GetWorld()->GetTimerManager().SetTimer(StopHandle,
		[&]() {
			GetWorld()->GetTimerManager().ClearTimer(ActivateHandle);
			// ActivateHandle.Invalidate();
		}, 1, false);
}

void AEKSanctuary::SaveMap()
{
	// Save Enemy Transform
	TArray<AActor*> EnemyArray;
	UGameplayStatics::GetAllActorsOfClass(this, AEK_EnemyBase::StaticClass(), EnemyArray);
	for (const auto actor : EnemyArray)
	{
		AEK_EnemyBase* enemy = Cast<AEK_EnemyBase>(actor);
		if (!enemy) continue;
		enemy->InitStat();

		if (enemy->SantuaryID == SanctuaryID)
		{
			if (enemy->EnemyStat && enemy->EnemyStat->GetIsBoss())
			{
				SavedBoss = enemy;
				SavedBossTransform = enemy->GetTransform();
			}
			else
			{
				SavedActors.Add(actor);
				PatrolRoutes.Add(enemy->GetPatrolRoute());
				SavedTransforms.Add(enemy->GetTransform());
				SavedClasses.Add(enemy->GetClass());
			}
		}
	}
}

void AEKSanctuary::LoadMap()
{
	// Load Boss State
	if (IsValid(SavedBoss) && !SavedBoss->GetFName().IsEqual(FName("None")))
	{
		AEK_EnemyBase* enemy = Cast<AEK_EnemyBase>(SavedBoss);
		if (!enemy)
		{
			UE_LOG(LogTemp, Warning, TEXT("AEKSanctuary : Failed to Get Saved Enemy."));
		}
		else
		{
			// Recover Start State
			enemy->RestoreState();

			// Set Transform
			enemy->StopAnimMontage();
		}
	}

	// Load Enemy Transform
	for (int i = 0; i < SavedActors.Num(); i++)
	{
		if (!SavedTransforms.IsValidIndex(i) || !SavedClasses.IsValidIndex(i))
		{
			continue;
		}

		const auto actor = SavedActors[i];

		if (IsValid(actor) && !actor->GetFName().IsEqual(FName("None")))
		{
			AEK_EnemyBase* enemy = Cast<AEK_EnemyBase>(actor);
			if (!enemy)
			{
				UE_LOG(LogTemp, Warning, TEXT("AEKSanctuary : Failed to Get Saved Enemy."));
				continue;
			}

			// Recover Start State
			enemy->RestoreState();

			// Set Transform
			enemy->StopAnimMontage();
		}
		else
		{
			AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(SavedClasses[i]);
			AEK_EnemyBase* enemy = Cast<AEK_EnemyBase>(spawnedActor);
			if (!enemy)
			{
				UE_LOG(LogTemp, Warning, TEXT("AEKSanctuary : Failed to Spawn Saved Enemy."));
				continue;
			}

			SavedActors[i] = enemy;

			// Set Patrol Route
			if (PatrolRoutes.IsValidIndex(i) && IsValid(PatrolRoutes[i]))
				enemy->SetPatrolRoute(PatrolRoutes[i]);

			// Recover Start State
			enemy->SpawnDefaultController();

			// Set Transform
			enemy->SetActorTransform(SavedTransforms[i], false, nullptr, ETeleportType::ResetPhysics);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(Handle_Transform,
		[&]() {
			// Load Boss Transform
			if (IsValid(SavedBoss) && !SavedBoss->GetFName().IsEqual(FName("None")))
			{
				SavedBoss->SetActorTransform(SavedBossTransform, false, nullptr, ETeleportType::ResetPhysics);
			}

			// Load Enemy Transform
			for (int i = 0; i < SavedActors.Num(); i++)
			{
				if (SavedActors[i])
					SavedActors[i]->SetActorTransform(SavedTransforms[i], false, nullptr, ETeleportType::ResetPhysics);
			}
		},
		0.5, false);
}