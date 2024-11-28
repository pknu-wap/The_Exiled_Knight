// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/Widget_GameMenu_Button.h"
#include "UI/Sanctuary/Widget_Sanctuary.h"
#include "Components/Button.h"
#include "UI/UISubsystem.h"
#include "EKGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Equipment/Widget_EquipSelectWindow.h"
#include "Components/InventoryComponent.h"

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
	case EGameMenuCategory::EquipMagic:
	{
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_Santuary, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_EquipMagic, ESlateVisibility::SelfHitTestInvisible);
		
		UUserWidget* widget = UIManager->GetWidget(FEKGameplayTags::Get().UI_Widget_GameMenu_EquipMagic);
		if (!widget) return;
		UWidget_EquipSelectWindow* userWidget = Cast<UWidget_EquipSelectWindow>(widget);
		if (!userWidget) return;
		userWidget->UpdateContents(EItemCategory::Magic);
		
		break;
	}
	case EGameMenuCategory::Map:
	{
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_Santuary, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_Map, ESlateVisibility::SelfHitTestInvisible);
		break;
	}
	case EGameMenuCategory::PT_Distribute:
	{
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_Santuary, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_Potion, ESlateVisibility::SelfHitTestInvisible);
		break;
	}
	case EGameMenuCategory::PT_Quantity:
	{
		APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
		if (!pc) return;
		UInventoryComponent* inventoryComp = pc->GetComponentByClass<UInventoryComponent>();
		if (!inventoryComp) return;

		UUserWidget* widget = UIManager->GetWidget(FEKGameplayTags::Get().UI_Widget_GameMenu_Santuary);
		if (!widget) return;
		UWidget_Sanctuary* userWidget = Cast<UWidget_Sanctuary>(widget);
		if (!userWidget) return;

		// inventoryComp->UpgradePotionCount()
		bool success = false;

		if (success)
		{
			userWidget->CountUpgrade_Success();
		}
		else
		{
			userWidget->CountUpgrade_Failed();
		}

		break;
	}
	case EGameMenuCategory::PT_HealRate:
	{
		APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
		if (!pc) return;
		UInventoryComponent* inventoryComp = pc->GetComponentByClass<UInventoryComponent>();
		if (!inventoryComp) return;

		UUserWidget* widget = UIManager->GetWidget(FEKGameplayTags::Get().UI_Widget_GameMenu_Santuary);
		if (!widget) return;
		UWidget_Sanctuary* userWidget = Cast<UWidget_Sanctuary>(widget);
		if (!userWidget) return;

		// inventoryComp->UpgradePotionCount()
		bool success = false;

		if (success)
		{
			userWidget->RateUpgrade_Success();
		}
		else
		{
			userWidget->RateUpgrade_Failed();
		}

		break;
	}
	case EGameMenuCategory::UpgradeStatus:
	{
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_Santuary, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_GameMenu_UpgradeStatus, ESlateVisibility::SelfHitTestInvisible);
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
