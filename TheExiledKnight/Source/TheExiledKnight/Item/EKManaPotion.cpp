// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EKManaPotion.h"
#include "Player/EKPlayer/EKPlayerStatusComponent.h"
#include "Subsystems/InventorySubsystem.h"

AEKManaPotion::AEKManaPotion()
{
}

void AEKManaPotion::BeginPlay()
{
	Super::BeginPlay();
}

void AEKManaPotion::UseItem(UWorld* WorldContext, float PotionRate)
{
	Super::UseItem(WorldContext, PotionRate);

	AEKPlayer* player = Cast<AEKPlayer>(UGameplayStatics::GetPlayerCharacter(WorldContext, 0));

	if (player != nullptr)
	{
		int32 HealAmount;
		HealAmount = player->GetPlayerStatusComponent()->GetMaxMp() * 0.4 * PotionRate;
		player->GetPlayerStatusComponent()->SetMp(HealAmount);
		UE_LOG(LogTemp, Warning, TEXT("MP HEAL : %d"), HealAmount);
	}
}
