// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/Widget_GameMenu_Button.h"
#include "Components/Button.h"
#include "UI/UISubsystem.h"
#include "EKGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UWidget_GameMenu_Button::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Menu->OnClicked.AddUniqueDynamic(this, &UWidget_GameMenu_Button::MenuButtonClicked);
}

void UWidget_GameMenu_Button::MenuButtonClicked()
{
	UUISubsystem* UIManager = GetGameInstance()->GetSubsystem<UUISubsystem>();
	if (!UIManager) return;

	switch (MenuCategory)
	{
	case EGameMenuCategory::Inventory:
	{
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_GameMenu, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_Inventory, ESlateVisibility::SelfHitTestInvisible);
		break;
	}
	case EGameMenuCategory::Equipment:
	{
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_GameMenu, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_Equipment, ESlateVisibility::SelfHitTestInvisible);
		break;
	}
	case EGameMenuCategory::Setting:
	{
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_GameMenu, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_Setting, ESlateVisibility::SelfHitTestInvisible);
		break;
	}
	case EGameMenuCategory::Quit:
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Type::Quit, false);
		break;
	}
	default:
		break;
	}
}
