// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SlotComponent.h"
#include "UI/UISubsystem.h"
#include "EKGameplayTags.h"
#include "UI/Equipment/Widget_Equipment.h"
#include "Subsystems/InventorySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/EKPlayer/EKPlayer.h"
#include "Player/EKPlayer/EKPlayerStatusComponent.h"

// Sets default values for this component's properties
USlotComponent::USlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USlotComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < MaxWeaponSlot; i++)
		WeaponSlots.Add(FItemStruct());
	for (int i = 0; i < MaxRuneSlot; i++)
		RuneSlots.Add(FItemStruct());
	for (int i = 0; i < MaxUseableSlot; i++)
		UseableSlots.Add(FItemStruct());
	for (int i = 0; i < MaxMagicSlot; i++)
		MagicSlots.Add(FEKPlayerMagic());
}


// Called every frame
void USlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USlotComponent::EquipWeapon(const FItemStruct& InItemData)
{
	UWidget_Equipment* equipWidget = GetEquipmentWidget();
	if (!equipWidget) return;

	UInventorySubsystem* inventorySystem =
		GetWorld()->GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!inventorySystem) return;

	FWeaponStruct* weaponInfo = inventorySystem->GetWeaponInfo(InItemData.ID);
	if (!weaponInfo) return;

	AEKPlayer* player = Cast<AEKPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!player) return;

	int slotIdx = equipWidget->GetEquipSelectSlotIdx();
	if (WeaponSlots.IsValidIndex(slotIdx))
	{
		WeaponSlots[slotIdx] = InItemData;

		UEKPlayerStatusComponent* statusComp = player->GetComponentByClass<UEKPlayerStatusComponent>();
		if (!statusComp) return;
		statusComp->Recalculate_Status();

		if (slotIdx == ActiveWeaponSlot)
		{
			player->EquipWeapon(*weaponInfo);
			Delegate_QuickSlotUpdated.Broadcast(EItemCategory::Weapon, ActiveWeaponSlot);
		}

		Delegate_SlotUpdated.Broadcast(EItemCategory::Weapon, slotIdx);
	}
}

void USlotComponent::EquipRune(const FItemStruct& InItemData)
{
	UWidget_Equipment* equipWidget = GetEquipmentWidget();
	if (!equipWidget) return;

	int slotIdx = equipWidget->GetEquipSelectSlotIdx();
	if (RuneSlots.IsValidIndex(slotIdx))
	{
		RuneSlots[slotIdx] = InItemData;
		Delegate_SlotUpdated.Broadcast(EItemCategory::Rune, slotIdx);
	}

	AEKPlayer* player = Cast<AEKPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!player) return;
	UEKPlayerStatusComponent* statusComp = player->GetComponentByClass<UEKPlayerStatusComponent>();
	if (!statusComp) return;
	statusComp->Recalculate_Status();
}

void USlotComponent::EquipUseableItem(const FItemStruct& InItemData)
{
	UWidget_Equipment* equipWidget = GetEquipmentWidget();
	if (!equipWidget) return;

	int slotIdx = equipWidget->GetEquipSelectSlotIdx();
	if (UseableSlots.IsValidIndex(slotIdx))
	{
		UseableSlots[slotIdx] = InItemData;
		Delegate_SlotUpdated.Broadcast(EItemCategory::UseableItem, slotIdx);

		if (slotIdx == ActiveUseableSlot)
		{
			Delegate_QuickSlotUpdated.Broadcast(EItemCategory::UseableItem, slotIdx);
		}
	}
}

void USlotComponent::EquipMagic(const FEKPlayerMagic& InMagicData)
{
	UWidget_Equipment* equipWidget = GetEquipmentWidget();
	if (!equipWidget) return;

	int slotIdx = equipWidget->GetEquipSelectSlotIdx();

	if (MagicSlots.IsValidIndex(slotIdx))
	{
		MagicSlots[slotIdx] = InMagicData;

		Delegate_SlotUpdated.Broadcast(EItemCategory::Magic, slotIdx);
		if (slotIdx == ActiveMagicSlot)
		{
			Delegate_QuickSlotUpdated.Broadcast(EItemCategory::Magic, ActiveMagicSlot);
		}
	}
}

void USlotComponent::UnEquip(EItemCategory InCategory, const FItemStruct& InItemData)
{
	UInventorySubsystem* inventorySystem =
		GetWorld()->GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!inventorySystem) return;

	AEKPlayer* player = Cast<AEKPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!player) return;

	UEKPlayerStatusComponent* statusComp = player->GetComponentByClass<UEKPlayerStatusComponent>();
	if (!statusComp) return;

	int idx = 0;

	switch (InCategory)
	{
	case EItemCategory::None:
		break;
	case EItemCategory::Weapon:
		for (int i = 0; i < WeaponSlots.Num(); i++)
		{
			if (WeaponSlots[i].ID == InItemData.ID && WeaponSlots[i].Name.IsEqual(InItemData.Name))
			{
				WeaponSlots[i] = FItemStruct();
				idx = i;

				if (ActiveWeaponSlot == i)
				{
					player->UnEquipWeapon();
				}

				break;
			}
		}
		break;
	case EItemCategory::Rune:
		for (int i = 0; i < RuneSlots.Num(); i++)
		{
			if (RuneSlots[i].ID == InItemData.ID && RuneSlots[i].Name.IsEqual(InItemData.Name))
			{
				RuneSlots[i] = FItemStruct();
				idx = i;
				break;
			}
		}
		break;
	case EItemCategory::FragmentOfGod:
		// if(Fragment)
		break;
	case EItemCategory::UseableItem:
		for (int i = 0; i < UseableSlots.Num(); i++)
		{
			if (UseableSlots[i].ID == InItemData.ID && UseableSlots[i].Name.IsEqual(InItemData.Name))
			{
				UseableSlots[i] = FItemStruct();
				idx = i;
				break;
			}
		}
		break;
	case EItemCategory::Magic:
		break;
	case EItemCategory::Upgrades:
		break;
	case EItemCategory::Hunting:
		break;
	default:
		break;
	}

	Delegate_SlotUpdated.Broadcast(InCategory, idx);
	Delegate_QuickSlotUpdated.Broadcast(InCategory, idx);
	statusComp->Recalculate_Status();
}

void USlotComponent::UnEquipMagic(EItemCategory InCategory, const FEKPlayerMagic& InMagicData)
{
	int idx = 0;
	for (int i = 0; i < MagicSlots.Num(); i++)
	{
		if (MagicSlots[i].MagicID == InMagicData.MagicID
			&& MagicSlots[i].MagicName.IsEqual(InMagicData.MagicName))
		{
			MagicSlots[i] = FEKPlayerMagic();
		}
	}
	Delegate_SlotUpdated.Broadcast(InCategory, idx);
	Delegate_QuickSlotUpdated.Broadcast(InCategory, idx);
}

bool USlotComponent::IsAlreadyEquiped(EItemCategory InCategory, const FItemStruct& InItemData)
{
	switch (InCategory)
	{
	case EItemCategory::None:
		break;
	case EItemCategory::Weapon:
		for (int i = 0; i < WeaponSlots.Num(); i++)
		{
			if (WeaponSlots[i].ID == InItemData.ID && WeaponSlots[i].Name.IsEqual(InItemData.Name))
				return true;
		}
		break;
	case EItemCategory::Rune:
		for (int i = 0; i < RuneSlots.Num(); i++)
		{
			if (RuneSlots[i].ID == InItemData.ID && RuneSlots[i].Name.IsEqual(InItemData.Name))
				return true;
		}
		break;
	case EItemCategory::FragmentOfGod:
		// if(Fragment)
		break;
	case EItemCategory::UseableItem:
		break;
	case EItemCategory::Magic:
		break;
	case EItemCategory::Upgrades:
		break;
	case EItemCategory::Hunting:
		break;
	default:
		break;
	}

	return false;
}

bool USlotComponent::IsAlreadyEquiped(EItemCategory InCategory, const FEKPlayerMagic& InMagicData)
{
	for (int i = 0; i < MagicSlots.Num(); i++)
	{
		if (MagicSlots[i].MagicID == InMagicData.MagicID 
			&& MagicSlots[i].MagicName.IsEqual(InMagicData.MagicName))
			return true;
	}
	return false;
}

void USlotComponent::UpdateActiveSlot(EInputType InInputType)
{
	switch (InInputType)
	{
	case EInputType::Up:
	{
		ActiveMagicSlot++;
		if (ActiveMagicSlot >= MaxMagicSlot)
			ActiveMagicSlot = 0;

		Delegate_QuickSlotUpdated.Broadcast(EItemCategory::Magic, ActiveMagicSlot);

		break;
	}
	case EInputType::Down:
	{
		ActiveUseableSlot++;
		if (ActiveUseableSlot >= MaxUseableSlot)
			ActiveUseableSlot = 0;

		Delegate_QuickSlotUpdated.Broadcast(EItemCategory::UseableItem, ActiveUseableSlot);

		break;
	}
	case EInputType::Left:
	{
		ActiveFragmentSlot++;
		if (ActiveFragmentSlot >= MaxFragmentSlot)
			ActiveFragmentSlot = 0;

		Delegate_QuickSlotUpdated.Broadcast(EItemCategory::FragmentOfGod, ActiveFragmentSlot);

		break;
	}
	case EInputType::Right:
	{
		ActiveWeaponSlot++;
		if (ActiveWeaponSlot >= MaxWeaponSlot)
			ActiveWeaponSlot = 0;

		UInventorySubsystem* inventorySystem =
			GetWorld()->GetGameInstance()->GetSubsystem<UInventorySubsystem>();
		if (!inventorySystem) return;

		FWeaponStruct* weaponInfo = inventorySystem->GetWeaponInfo(WeaponSlots[ActiveWeaponSlot].ID);
		if (!weaponInfo) return;

		AEKPlayer* player = Cast<AEKPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (!player) return;

		player->EquipWeapon(*weaponInfo);

		Delegate_QuickSlotUpdated.Broadcast(EItemCategory::Weapon, ActiveWeaponSlot);

		break;
	}
	default:
		break;
	}
}

UWidget_Equipment* USlotComponent::GetEquipmentWidget()
{
	UUISubsystem* UISystem = GetWorld()->GetGameInstance()->GetSubsystem<UUISubsystem>();
	if (!UISystem) return nullptr;

	UUserWidget* widget = UISystem->GetWidget(FEKGameplayTags::Get().UI_Widget_GameMenu_Equipment);
	if (!widget) return nullptr;

	UWidget_Equipment* equipWidget = Cast<UWidget_Equipment>(widget);
	return equipWidget;
}