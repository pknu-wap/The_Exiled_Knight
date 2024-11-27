// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_BossBattle.generated.h"

class AEK_EnemyBase;
class UProgressBar;
class UTextBlock;

UCLASS()
class THEEXILEDKNIGHT_API UWidget_BossBattle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void StartBossBattle(AEK_EnemyBase* InBoss);
	void EndBossBattle();

public:
	UFUNCTION()
	void BossHPUpdated(int MaxHP, int CurrentHP);

	UFUNCTION()
	void BossDied();

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UProgressBar* ProgressBar_HP;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UTextBlock* Text_BossName;
};
