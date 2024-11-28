// Made by Somalia Pirate

#include "EKPlayerStatusComponent.h"
#include "../Weapon/EKPlayerWeapon.h"
#include "../EKPlayerGameplayTags.h"
#include "Components/SlotComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/InventorySubsystem.h"

UEKPlayerStatusComponent::UEKPlayerStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Edit Basic Status Value Here
	MaxHp = PlayerMaxHp;
	Hp = MaxHp;
	MaxMp = PlayerMaxMp;
	Mp = MaxMp;
	MaxStamina = PlayerMaxStamina;
	Stamina = MaxStamina;
	DefaultDamage = 10.f;
	FinalDamage = DefaultDamage;

	// Edit Basic Status Value Here
	Level = 1;
	Vitality = 1; BaseVitality = 1;
	Mental = 1;	BaseMental = 1;
	Endurance = 1; BaseEndurance = 1;
	Strength = 1; BaseStrength = 1;
	Ability = 1; BaseAbility = 1;
	Intelligence = 1; BaseIntelligence = 1;
}


void UEKPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	EKPlayer = Cast<AEKPlayer>(GetOwner());

	if (EKPlayer)
	{
		EKPlayerController = Cast<AEKPlayerController>(EKPlayer->GetController());
	}

	EKPlayerGameInstance = Cast<UEKPlayerGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (EKPlayerGameInstance)
	{
		FEKPlayerLevel* EKPlayerLevelTemp = EKPlayerGameInstance->GetEKPlayerLevelData(Level);
		EKPlayerLevelData = *EKPlayerLevelTemp;

		FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(Level);
		EKPlayerStatusData = *EKPlayerStatusTemp;
	}

	Recalculate_Status();
	SetStatusHandle();
}

void UEKPlayerStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Stamina Recovery
	if (bCanStaminaRecovery)
	{
		SetStamina(4);
	}
}

#pragma region Set Basic Status

void UEKPlayerStatusComponent::SetStatusHandle()
{
	SetMaxHp();
	SetMaxMp();
	SetMaxStamina();
	SetPlayerDefaultDamage();
	SetPlayerFinalDamage();
}

void UEKPlayerStatusComponent::SetMaxHp()
{
	if (!EKPlayerGameInstance)
	{
		return;
	}

	FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(BaseVitality);
	EKPlayerStatusData = *EKPlayerStatusTemp;

	MaxHp = EKPlayerStatusData.Vitality;
	SetHp(MaxHp);
}

void UEKPlayerStatusComponent::SetHp(int32 SetData)
{
	Hp = FMath::Clamp(Hp + SetData, 0, MaxHp);
	Delegate_HPUpdated.Broadcast(MaxHp, Hp);
}

void UEKPlayerStatusComponent::SetMaxMp()
{
	if (!EKPlayerGameInstance)
	{
		return;
	}

	FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(BaseMental);
	EKPlayerStatusData = *EKPlayerStatusTemp;

	MaxMp = EKPlayerStatusData.Mental;
	SetMp(MaxMp);
}

void UEKPlayerStatusComponent::SetMp(int32 SetData)
{
	Mp = FMath::Clamp(Mp + SetData, 0, MaxMp);
	Delegate_MPUpdated.Broadcast(MaxMp, Mp);
}

void UEKPlayerStatusComponent::SetMaxStamina()
{
	if (!EKPlayerGameInstance)
	{
		return;
	}

	FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(BaseEndurance);
	EKPlayerStatusData = *EKPlayerStatusTemp;

	MaxStamina = EKPlayerStatusData.Endurance;
	SetStamina(MaxStamina);
}

void UEKPlayerStatusComponent::SetStamina(int32 SetData)
{
	Stamina = FMath::Clamp(Stamina + SetData, 0, MaxStamina);
	Delegate_StaminaUpdated.Broadcast(MaxStamina, Stamina);
}

void UEKPlayerStatusComponent::RestoreState()
{
	SetHp(MaxHp);
	SetMp(MaxMp);
	SetStamina(MaxStamina);
}

void UEKPlayerStatusComponent::Recalculate_Status()
{
	Calculate_NormalStatus();
	Calculate_BasicStatus();
	SetPlayerDefaultDamage();
	SetPlayerFinalDamage();
	ATK = FinalDamage;

	Delegate_HPUpdated.Broadcast(MaxHp, Hp);
	Delegate_MPUpdated.Broadcast(MaxMp, Mp);
	Delegate_StaminaUpdated.Broadcast(MaxStamina, Stamina);
}

void UEKPlayerStatusComponent::Calculate_BasicStatus()
{
	SetMaxHp();
	SetMaxMp();
	SetMaxStamina();

	UInventorySubsystem* invSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!invSystem) return;
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	if (!pc) return;
	USlotComponent* slotComp = pc->GetComponentByClass<USlotComponent>();
	if (!slotComp) return;

	DEF = 7.5 * Endurance + 2.5 * Ability;

	// Calculate Weapon Stat
	if (slotComp->WeaponSlots.IsValidIndex(slotComp->ActiveWeaponSlot))
	{
		FWeaponStruct* weaponInfo = invSystem->GetWeaponInfo(slotComp->WeaponSlots[slotComp->ActiveWeaponSlot].ID);
		if (!weaponInfo) return;
		ATK = weaponInfo->AttackPow + 0.2 * (weaponInfo->STRRate * Strength + weaponInfo->DEXRate * Ability + weaponInfo->INTRate * Intelligence);
	}
	else
	{
		ATK = 0;
	}
}

void UEKPlayerStatusComponent::Calculate_NormalStatus()
{
	// Calculate Rune Stat
	UInventorySubsystem* invSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!invSystem) return;
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	if (!pc) return;
	USlotComponent* slotComp = pc->GetComponentByClass<USlotComponent>();
	if (!slotComp) return;

	Vitality = BaseVitality;
	Mental = BaseMental;
	Endurance = BaseEndurance;
	Strength = BaseStrength;
	Ability = BaseAbility;
	Intelligence = BaseIntelligence;

	for (int i = 0; i < slotComp->RuneSlots.Num(); i++)
	{
		int itemID = slotComp->RuneSlots[i].ID;
		if (itemID > 1)
		{
			FRune* runeInfo = invSystem->GetRuneInfo(itemID);
			if (!runeInfo)
				continue;

			Vitality += runeInfo->Vitality;
			Mental += runeInfo->Mental;
			Endurance += runeInfo->Endurance;
			Strength += runeInfo->Strength;
			Ability += runeInfo->Ability;
			Intelligence += runeInfo->Intelligence;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT(""))
}

int UEKPlayerStatusComponent::GetCalculatedHP(int InVitality)
{
	FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(InVitality);
	if (!EKPlayerStatusTemp) return 0;
	int32 CalculatedHP = EKPlayerStatusTemp->Vitality;
	return CalculatedHP;
}

int UEKPlayerStatusComponent::GetCalculatedMP(int InMental)
{
	FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(InMental);
	if (!EKPlayerStatusTemp) return 0;
	int32 CalculatedMP = EKPlayerStatusTemp->Mental;
	return CalculatedMP;
}

int UEKPlayerStatusComponent::GetCalculatedStamina(int InEndurance)
{
	FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(InEndurance);
	if (!EKPlayerStatusTemp) return 0;
	int32 CalculatedStamina = EKPlayerStatusTemp->Endurance;
	return CalculatedStamina;
}

int UEKPlayerStatusComponent::GetCalculatedATK(int InStrength, int InAbility, int InIntelligence)
{
	// Calculate 
	int calculated = 0;
	if (!EKPlayerGameInstance || !EKPlayer->GetCurrentWeapon())
	{
		return calculated;
	}

	UInventorySubsystem* invSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!invSystem) return calculated;
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	if (!pc) return calculated;
	USlotComponent* slotComp = pc->GetComponentByClass<USlotComponent>();
	if (!slotComp) return calculated;

	if (slotComp->WeaponSlots.IsValidIndex(slotComp->ActiveWeaponSlot))
	{
		FItemStruct currentWeapon = *invSystem->GetItemInfo(slotComp->WeaponSlots[slotComp->ActiveWeaponSlot].ID);

		if (EKPlayer->EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_GreatSword))
		{
			FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(InStrength);
			EKPlayerStatusData = *EKPlayerStatusTemp;
			calculated = DefaultDamage + EKPlayerStatusData.Strength + EKPlayer->GetCurrentWeapon()->WeaponAdditionalDamage;
			calculated *= invSystem->GetLevelRateInfo(currentWeapon.ItemLevel)->SwordRate;
		}
		else if (EKPlayer->EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_Spear))
		{
			FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(InAbility);
			EKPlayerStatusData = *EKPlayerStatusTemp;
			calculated = DefaultDamage + EKPlayerStatusData.Ability + EKPlayer->GetCurrentWeapon()->WeaponAdditionalDamage;
			calculated *= invSystem->GetLevelRateInfo(currentWeapon.ItemLevel)->SpearRate;
		}
		else if (EKPlayer->EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_Staff))
		{
			FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(InIntelligence);
			EKPlayerStatusData = *EKPlayerStatusTemp;
			calculated = DefaultDamage + EKPlayerStatusData.Intelligence + EKPlayer->GetCurrentWeapon()->WeaponAdditionalDamage;
			calculated *= invSystem->GetLevelRateInfo(currentWeapon.ItemLevel)->StaffRate;
		}
	}

	return calculated;
}

int UEKPlayerStatusComponent::GetCalculatedDEF(int InEndurance, int InAbility)
{
	return (7.5 * (double)InEndurance + 2.5 * (double)InAbility);
}

#pragma endregion

#pragma region Damage

void UEKPlayerStatusComponent::SetPlayerDefaultDamage()
{
	FEKPlayerLevel* EKPlayerLevelDataTemp = EKPlayerGameInstance->GetEKPlayerLevelData(Level);
	EKPlayerLevelData = *EKPlayerLevelDataTemp;
	DefaultDamage = EKPlayerLevelData.DefaultDamage;
}

void UEKPlayerStatusComponent::SetPlayerFinalDamage()
{
	if (!EKPlayerGameInstance || !EKPlayer->GetCurrentWeapon())
	{
		FinalDamage = 0;
		ATK = FinalDamage;
		return;
	}

	UInventorySubsystem* invSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!invSystem) return;
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	if (!pc) return;
	USlotComponent* slotComp = pc->GetComponentByClass<USlotComponent>();
	if (!slotComp) return;

	if (slotComp->WeaponSlots.IsValidIndex(slotComp->ActiveWeaponSlot))
	{
		FItemStruct currentWeapon = *invSystem->GetItemInfo(slotComp->WeaponSlots[slotComp->ActiveWeaponSlot].ID);

		if (EKPlayer->EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_GreatSword))
		{
			FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(BaseStrength);
			EKPlayerStatusData = *EKPlayerStatusTemp;
			FinalDamage = DefaultDamage + EKPlayerStatusData.Strength + EKPlayer->GetCurrentWeapon()->WeaponAdditionalDamage;
			FinalDamage *= invSystem->GetLevelRateInfo(currentWeapon.ItemLevel)->SwordRate;
		}
		else if (EKPlayer->EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_Spear))
		{
			FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(BaseAbility);
			EKPlayerStatusData = *EKPlayerStatusTemp;
			FinalDamage = DefaultDamage + EKPlayerStatusData.Ability + EKPlayer->GetCurrentWeapon()->WeaponAdditionalDamage;
			FinalDamage *= invSystem->GetLevelRateInfo(currentWeapon.ItemLevel)->SpearRate;
		}
		else if (EKPlayer->EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_Staff))
		{
			FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(BaseIntelligence);
			EKPlayerStatusData = *EKPlayerStatusTemp;
			FinalDamage = DefaultDamage + EKPlayerStatusData.Intelligence + EKPlayer->GetCurrentWeapon()->WeaponAdditionalDamage;
			FinalDamage *= invSystem->GetLevelRateInfo(currentWeapon.ItemLevel)->StaffRate;
		}

		ATK = FinalDamage;
	}
	else
	{
		ATK = 0;
	}
}

#pragma endregion

#pragma region LevelUp

void UEKPlayerStatusComponent::LevelUp(uint8 IncreaseLevel)
{
	if (!EKPlayerGameInstance)
	{
		return;
	}

	for (int i = 0; i < IncreaseLevel; i++)
	{
		FEKPlayerLevel* EKPlayerLevelDataTemp = EKPlayerGameInstance->GetEKPlayerLevelData(Level);
		EKPlayerLevelData = *EKPlayerLevelDataTemp;

		if (CurrentAstral >= EKPlayerLevelData.NeedAstral)
		{
			Level = FMath::Clamp(Level + 1, 0, PlayerMaxLevel);
			CurrentAstral -= EKPlayerLevelData.NeedAstral;
		}
	}
}

void UEKPlayerStatusComponent::LevelUpVitality(uint8 IncreaseLevel)
{
	BaseVitality = FMath::Clamp(BaseVitality + IncreaseLevel, 0, PlayerMaxVitalityLevel);
	SetMaxHp();
}

void UEKPlayerStatusComponent::LevelUpMental(uint8 IncreaseLevel)
{
	BaseMental = FMath::Clamp(BaseMental + IncreaseLevel, 0, PlayerMaxMentalLevel);
	SetMaxMp();
}

void UEKPlayerStatusComponent::LevelUpEndurance(uint8 IncreaseLevel)
{
	BaseEndurance = FMath::Clamp(BaseEndurance + IncreaseLevel, 0, PlayerMaxEnduranceLevel);
	SetMaxStamina();
}

void UEKPlayerStatusComponent::LevelUpStrength(uint8 IncreaseLevel)
{
	BaseStrength = FMath::Clamp(BaseStrength + IncreaseLevel, 0, PlayerMaxStrengthLevel);
	SetPlayerFinalDamage();
}

void UEKPlayerStatusComponent::LevelUpAbility(uint8 IncreaseLevel)
{
	BaseAbility = FMath::Clamp(BaseAbility + IncreaseLevel, 0, PlayerMaxAbilityLevel);
	SetPlayerFinalDamage();
}

void UEKPlayerStatusComponent::LevelUpIntelligence(uint8 IncreaseLevel)
{
	BaseIntelligence = FMath::Clamp(BaseIntelligence + IncreaseLevel, 0, PlayerMaxInteligenceLevel);
	SetPlayerFinalDamage();
}

#pragma endregion