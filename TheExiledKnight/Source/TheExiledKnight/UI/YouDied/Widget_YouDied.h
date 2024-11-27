// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_YouDied.generated.h"

/**
 * 
 */
UCLASS()
class THEEXILEDKNIGHT_API UWidget_YouDied : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PlayerDied();

public:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "UI", meta = (BindWidgetAnim))
	class UWidgetAnimation* Animation_Died;
};
