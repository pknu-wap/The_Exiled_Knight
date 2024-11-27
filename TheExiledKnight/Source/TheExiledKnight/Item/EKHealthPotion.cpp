// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EKHealthPotion.h"
#include "Player/EKPlayer/EKPlayerStatusComponent.h"
#include "Subsystems/InventorySubsystem.h"

AEKHealthPotion::AEKHealthPotion()
{
}

void AEKHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}

void AEKHealthPotion::UseItem(UWorld* WorldContext, float PotionRate)
{
	Super::UseItem(WorldContext, PotionRate);

	AEKPlayer* player = Cast<AEKPlayer>(UGameplayStatics::GetPlayerCharacter(WorldContext, 0));

	if (player != nullptr)
	{
		int32 HealAmount;
		HealAmount = player->GetPlayerStatusComponent()->GetMaxHp() * 0.2 * PotionRate;
		player->GetPlayerStatusComponent()->SetHp(HealAmount);
		UE_LOG(LogTemp, Warning, TEXT("HP HEAL : %d"), HealAmount);
	}
}
