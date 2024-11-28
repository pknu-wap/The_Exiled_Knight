// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Sanctuary.generated.h"

/**
 * 
 */
UCLASS()
class THEEXILEDKNIGHT_API UWidget_Sanctuary : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void CountUpgrade_Success();
	void CountUpgrade_Failed();
	void RateUpgrade_Success();
	void RateUpgrade_Failed();

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "UI", meta = (BindWidgetAnim))
	class UWidgetAnimation* Potion_CountUpgrade_Success;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "UI", meta = (BindWidgetAnim))
	class UWidgetAnimation* Potion_CountUpgrade_Failed;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "UI", meta = (BindWidgetAnim))
	class UWidgetAnimation* Potion_Upgrade_Success;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "UI", meta = (BindWidgetAnim))
	class UWidgetAnimation* Potion_Upgrade_Failed;
};
