// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Equipment/Widget_EquipSelect_ContentSlot.h"
#include "UI/Equipment/Widget_EquipSelectWindow.h"
#include "UI/Equipment/Widget_Equipment.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/SlotComponent.h"
#include "UI/UISubsystem.h"
#include "EKGameplayTags.h"
#include "Player/EKPlayer/EKPlayer.h"
#include "Player/EKPlayer/EKPlayerStatusComponent.h"

void UWidget_EquipSelect_ContentSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_EquipSelect_ContentSlot::UpdateSlot(EItemCategory inCategory, const FEKPlayerMagic& inData)
{
	Category = inCategory;
	MagicData = inData;

	Image_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Image_Item->SetBrushFromTexture(MagicData.Icon);
	Text_Quantity->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_EquipSelect_ContentSlot::UpdateSlot(EItemCategory inCategory, const FInventorySlot& inData)
{
	Category = inCategory;
	SlotData = inData;

	if (inData.Quantity > 0)
	{
		Image_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Image_Item->SetBrushFromTexture(SlotData.Item.Icon);

		Text_Quantity->SetText(UKismetTextLibrary::Conv_IntToText(inData.Quantity));
		Text_Quantity->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Image_Item->SetVisibility(ESlateVisibility::Collapsed);
		Text_Quantity->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWidget_EquipSelect_ContentSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	UUISubsystem* UISystem = GetWorld()->GetGameInstance()->GetSubsystem<UUISubsystem>();
	if (!UISystem) return;
	UUserWidget* widget = UISystem->GetWidget(FEKGameplayTags::Get().UI_Widget_GameMenu_EquipSelect);
	if (!widget) return;
	UWidget_EquipSelectWindow* EquipSelectWidget = Cast<UWidget_EquipSelectWindow>(widget);
	if (!EquipSelectWidget) return;

	widget = UISystem->GetWidget(FEKGameplayTags::Get().UI_Widget_GameMenu_EquipMagic);
	if (!widget) return;
	UWidget_EquipSelectWindow* EquipMagicWidget = Cast<UWidget_EquipSelectWindow>(widget);
	if (!EquipMagicWidget) return;

	switch (Category)
	{
	case EItemCategory::None:
		break;
	case EItemCategory::Weapon:
		EquipSelectWidget->UpdateDescription_Weapon(SlotData);
		break;
	case EItemCategory::Rune:
		EquipSelectWidget->UpdateDescription_Rune(SlotData);
		break;
	case EItemCategory::FragmentOfGod:
		break;
	case EItemCategory::UseableItem:
		EquipSelectWidget->UpdateDescription_Item(SlotData);
		break;
	case EItemCategory::Magic:
		EquipSelectWidget->UpdateDescription_Magic(MagicData);
		break;
	case EItemCategory::Upgrades:
		break;
	case EItemCategory::Hunting:
		break;
	default:
		break;
	}
}

FEventReply UWidget_EquipSelect_ContentSlot::RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (Category != EItemCategory::Magic && (SlotData.Item.ID == 0 || SlotData.Item.ID == 1)) return Reply;
	if (Category == EItemCategory::Magic && MagicData.MagicID < 0) return Reply;

	APlayerController* controller = GetOwningPlayer();
	if (!controller) return Reply;
	USlotComponent* slotComp = controller->GetComponentByClass<USlotComponent>();
	if (!slotComp) return Reply;

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		if (Category == EItemCategory::Magic && slotComp->IsAlreadyEquiped(EItemCategory::Magic, MagicData))
			slotComp->UnEquipMagic(EItemCategory::Magic, MagicData);

		else if (Category == EItemCategory::Weapon && slotComp->IsAlreadyEquiped(EItemCategory::Weapon, SlotData.Item))
			slotComp->UnEquip(EItemCategory::Weapon, SlotData.Item);

		else if (Category == EItemCategory::Rune && slotComp->IsAlreadyEquiped(EItemCategory::Rune, SlotData.Item))
			slotComp->UnEquip(EItemCategory::Rune, SlotData.Item);

		else if (Category == EItemCategory::FragmentOfGod && slotComp->IsAlreadyEquiped(EItemCategory::FragmentOfGod, SlotData.Item))
			slotComp->UnEquip(EItemCategory::FragmentOfGod, SlotData.Item);

		else if (Category == EItemCategory::UseableItem && slotComp->IsAlreadyEquiped(EItemCategory::UseableItem, SlotData.Item))
			slotComp->UnEquip(EItemCategory::UseableItem, SlotData.Item);

		// Equip Item
		switch (Category)
		{
		case EItemCategory::None:
		{
			break;
		}
		case EItemCategory::Weapon:
		{
			// Equip Weapon
			slotComp->EquipWeapon(SlotData.Item);
			break;
		}
		case EItemCategory::Rune:
		{
			// Equip Rune
			slotComp->EquipRune(SlotData.Item);
			break;
		}
		case EItemCategory::FragmentOfGod:
		{
			break;
		}
		case EItemCategory::UseableItem:
		{
			// Equip Useable Item
			slotComp->EquipUseableItem(SlotData.Item);
			break;
		}
		case EItemCategory::Magic:
		{
			slotComp->EquipMagic(MagicData);
			break;
		}
		default:
		{

			break;
		}
		}

	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		// UnEquip Item

		if (Category == EItemCategory::Magic && slotComp->IsAlreadyEquiped(EItemCategory::Magic, MagicData))
			slotComp->UnEquipMagic(EItemCategory::Magic, MagicData);

		else if (Category == EItemCategory::Weapon && slotComp->IsAlreadyEquiped(EItemCategory::Weapon, SlotData.Item))
			slotComp->UnEquip(EItemCategory::Weapon, SlotData.Item);

		else if (Category == EItemCategory::Rune && slotComp->IsAlreadyEquiped(EItemCategory::Rune, SlotData.Item))
			slotComp->UnEquip(EItemCategory::Rune, SlotData.Item);

		else if (Category == EItemCategory::FragmentOfGod && slotComp->IsAlreadyEquiped(EItemCategory::FragmentOfGod, SlotData.Item))
			slotComp->UnEquip(EItemCategory::FragmentOfGod, SlotData.Item);

		else if(Category == EItemCategory::UseableItem && slotComp->IsAlreadyEquiped(EItemCategory::UseableItem, SlotData.Item))
			slotComp->UnEquip(EItemCategory::UseableItem, SlotData.Item);
	}


	ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!player) return Reply;
	UEKPlayerStatusComponent* statusComp = player->GetComponentByClass<UEKPlayerStatusComponent>();
	if (!statusComp) return Reply;
	UUISubsystem* UISystem = GetWorld()->GetGameInstance()->GetSubsystem<UUISubsystem>();
	if (!UISystem) return Reply;
	UUserWidget* widget = UISystem->GetWidget(FEKGameplayTags::Get().UI_Widget_GameMenu_Equipment);
	if (!widget) return Reply;
	UWidget_Equipment* EquipmentWidget = Cast<UWidget_Equipment>(widget);
	if (!EquipmentWidget) return Reply;

	statusComp->Recalculate_Status();
	EquipmentWidget->UpdateStatusInfo();

	return Reply;
}
