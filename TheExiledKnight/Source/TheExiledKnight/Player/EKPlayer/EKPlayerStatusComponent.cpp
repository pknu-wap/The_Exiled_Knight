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
	MaxHp = 1000;
	Hp = 1000;
	MaxMp = 1000;
	Mp = 1000;
	MaxStamina = 1000;
	Stamina = 1000;
	
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

	Recalculate_Status();
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

	MaxHp = FMath::Clamp(EKPlayerStatusData.Vitality, 0, PlayerMaxHp);
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

	MaxMp = FMath::Clamp(EKPlayerStatusData.Mental, 0, PlayerMaxMp);
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

	MaxStamina = FMath::Clamp(EKPlayerStatusData.Endurance, 0, PlayerMaxStamina);
	SetStamina(MaxStamina);
}

void UEKPlayerStatusComponent::SetStamina(int32 SetData)
{
	Stamina = FMath::Clamp(Stamina + SetData, 0, MaxStamina);
	Delegate_StaminaUpdated.Broadcast(MaxStamina, Stamina);
}

void UEKPlayerStatusComponent::Recalculate_Status()
{
	Calculate_NormalStatus();
	Calculate_BasicStatus();
}

void UEKPlayerStatusComponent::Calculate_BasicStatus()
{
	MaxHp = 1000 + Vitality * 100;
	MaxMp = 1000 + Mental * 100;
	MaxStamina = 1000 + Endurance * 100;

	UInventorySubsystem* invSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!invSystem) return;
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	if (!pc) return;
	USlotComponent* slotComp = pc->GetComponentByClass<USlotComponent>();
	if (!slotComp) return;

	DEF = 75 * Endurance + 25 * Ability;

	// Calculate Weapon Stat
	FWeaponStruct* weaponInfo = invSystem->GetWeaponInfo(slotComp->WeaponSlots[slotComp->ActiveWeaponSlot].ID);
	if (!weaponInfo) return;
	ATK = weaponInfo->AttackPow;
	
	// Calculate 
	ATK *= 1 + 0.02 * Strength;
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
		if (itemID > 0)
		{
			FRune* runeInfo = invSystem->GetRuneInfo(itemID);
			if (!runeInfo) return;

			Vitality += runeInfo->Vitality;
			Mental += runeInfo->Mental;
			Endurance += runeInfo->Endurance;
			Strength += runeInfo->Strength;
			Ability += runeInfo->Ability;
			Intelligence += runeInfo->Intelligence;
		}
	}
}

int UEKPlayerStatusComponent::GetCalculatedHP(int InVitality)
{
	float rate = 1 + 0.02 * InVitality;
	float CalculatedHP = PlayerMaxHp + 100 * InVitality;
	return CalculatedHP;
}

int UEKPlayerStatusComponent::GetCalculatedMP(int InMental)
{
	float rate = 1 + 0.02 * InMental;
	float CalculatedMP = PlayerMaxMp + 100 * InMental;
	return CalculatedMP;
}

int UEKPlayerStatusComponent::GetCalculatedStamina(int InEndurance)
{
	float rate = 1 + 0.02 * InEndurance;
	float CalculatedStamina = PlayerMaxStamina + 100 * InEndurance;
	return CalculatedStamina;
}

int UEKPlayerStatusComponent::GetCalculatedATK(int InStrength)
{
	// Calculate 
	float rate = 1 + 0.02 * InStrength;
	float CalculatedATK = ATK * rate;
	return CalculatedATK;
}

int UEKPlayerStatusComponent::GetCalculatedDEF(int InEndurance, int InAbility)
{
	return 75 * InEndurance + 25 * InAbility;
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
	if (!EKPlayerGameInstance)
	{
		return;
	}

	if (EKPlayer->EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_GreatSword))
	{
		FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(BaseStrength);
		EKPlayerStatusData = *EKPlayerStatusTemp;
		FinalDamage = DefaultDamage + EKPlayerStatusData.Strength + EKPlayer->GetCurrentWeapon()->WeaponAdditionalDamage;
	}
	else if (EKPlayer->EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_Spear))
	{
		FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(BaseAbility);
		EKPlayerStatusData = *EKPlayerStatusTemp;
		FinalDamage = DefaultDamage + EKPlayerStatusData.Ability + EKPlayer->GetCurrentWeapon()->WeaponAdditionalDamage;
	}
	else if (EKPlayer->EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_Staff))
	{
		FEKPlayerStatus* EKPlayerStatusTemp = EKPlayerGameInstance->GetEKPlayerStatusData(BaseIntelligence);
		EKPlayerStatusData = *EKPlayerStatusTemp;
		FinalDamage = DefaultDamage + EKPlayerStatusData.Intelligence + EKPlayer->GetCurrentWeapon()->WeaponAdditionalDamage;
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