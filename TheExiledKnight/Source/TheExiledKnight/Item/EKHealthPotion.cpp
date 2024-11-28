// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EKHealthPotion.h"
#include "Player/EKPlayer/EKPlayerStatusComponent.h"
#include "Subsystems/InventorySubsystem.h"
#include "Player/EKPlayerGameplayTags.h"

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
		HealAmount = player->GetPlayerStatusComponent()->GetMaxHp() * 0.4 * PotionRate;
		player->GetPlayerStatusComponent()->SetHp(HealAmount);
		UE_LOG(LogTemp, Warning, TEXT("HP HEAL : %d"), HealAmount);
		player->EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_UseItem);
	}
}
