// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossBattle/Widget_BossBattle.h"
#include "Enemy/EK_EnemyBase.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Enemy/EK_EnemyStatusComponent.h"

void UWidget_BossBattle::StartBossBattle(AEK_EnemyBase* InBoss)
{
	if (!IsValid(InBoss)) return;
	InBoss->Delegate_HpUpdated.AddUObject(this, &UWidget_BossBattle::BossHPUpdated);
	InBoss->Delegate_Died.AddUObject(this, &UWidget_BossBattle::BossDied);

	UEK_EnemyStatusComponent* StatusComp = InBoss->GetStatusComponent();
	if (!StatusComp) return;
	float currentHP = StatusComp->GetCurrentHealth();
	float maxHP = StatusComp->GetMaxHealth();

	ProgressBar_HP->SetPercent(currentHP / maxHP);
}

void UWidget_BossBattle::EndBossBattle()
{
}

void UWidget_BossBattle::BossHPUpdated(int MaxHP, int CurrentHP)
{
	float percent = float(CurrentHP) / float(MaxHP);
	ProgressBar_HP->SetPercent(percent);
}

void UWidget_BossBattle::BossDied()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
