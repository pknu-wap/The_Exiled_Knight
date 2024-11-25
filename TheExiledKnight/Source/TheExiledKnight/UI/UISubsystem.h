// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagsManager.h"
#include "UISubsystem.generated.h"

USTRUCT(BlueprintType)
struct FTagWidgetStruct
{
	GENERATED_BODY()

	FTagWidgetStruct() {}
	FTagWidgetStruct(FGameplayTag InTag, UUserWidget* InWidget) : Tag(InTag), Widget(InWidget)
	{}

	FGameplayTag Tag;
	UUserWidget* Widget;
};

UCLASS()
class THEEXILEDKNIGHT_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UFUNCTION(BlueprintCallable)
	void RegisterLayer(FGameplayTag LayerTag, UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void UnRegisterLayer(FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable)
	void RegisterWidget(FGameplayTag WidgetTag, UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void UnRegisterWidget(FGameplayTag WidgetTag);

	UFUNCTION(BlueprintCallable)
	void SetLayerVisibility(FGameplayTag LayerTag, ESlateVisibility Visibility);

	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetLayerVisibility(FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable)
	void SetWidgetVisibility(FGameplayTag WidgetTag, ESlateVisibility Visibility);

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetLayer(FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetWidget(FGameplayTag WidgetTag);


private:
	// Layers Map
	TMap<FGameplayTag, UUserWidget*> LayerMap;

	// All Widgets Map
	TMap<FGameplayTag, UUserWidget*> WidgetMap;
};
