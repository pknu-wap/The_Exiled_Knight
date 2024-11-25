// Made by Somalia Pirate

#include "Spear.h"

ASpear::ASpear()
{
	PrimaryActorTick.bCanEverTick = true;

	Spear = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Spear"));
	RootComponent = Spear;

	Spear->SetSkeletalMesh(SpearMesh);

	WeaponCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	WeaponCapsuleComponent->SetupAttachment(RootComponent);
	WeaponCapsuleComponent->SetRelativeLocationAndRotation(FVector(30, 0, 0), FRotator(-90, 0, 0));
	WeaponCapsuleComponent->SetRelativeScale3D(FVector(1.f, 1.f, 3.f));
	WeaponCapsuleComponent->SetCapsuleRadius(11.f);
	WeaponCapsuleComponent->SetCapsuleHalfHeight(44.f);

	MaxAttackCombo = 5;

	WeaponAdditionalDamage = 10;
}

void ASpear::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Attack

void ASpear::PlayAttackStartAnimMontage(AEKPlayer* EKPlayer, AEKPlayerController* EKPlayerController)
{
	if (!EKPlayerController->bIsEquipWeapon || !SpearAttackAnim)
	{
		return;
	}

	if (EKPlayer->GetPlayerStatusComponent()->GetStamina() < SpearAttackStamina)
	{
		EKPlayer->EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Attack);
		return;
	}

	if (AttackCombo == 1)
	{
		EKPlayer->StopAnimMontage(SpearAttackAnim);
		EKPlayer->PlayAnimMontage(SpearAttackAnim, 1.0f, FName("Attack1"));
		EKPlayerController->SetAttackEndTimer(1.0f);
	}
	else if (AttackCombo == 2)
	{
		EKPlayer->StopAnimMontage(SpearAttackAnim);
		EKPlayer->PlayAnimMontage(SpearAttackAnim, 1.0f, FName("Attack2"));
		EKPlayerController->SetAttackEndTimer(1.0f);
	}
	else if (AttackCombo == 3)
	{
		EKPlayer->StopAnimMontage(SpearAttackAnim);
		EKPlayer->PlayAnimMontage(SpearAttackAnim, 1.0f, FName("Attack3"));
		EKPlayerController->SetAttackEndTimer(0.83f);
	}
	else if (AttackCombo == 4)
	{
		EKPlayer->StopAnimMontage(SpearAttackAnim);
		EKPlayer->PlayAnimMontage(SpearAttackAnim, 1.0f, FName("Attack4"));
		EKPlayerController->SetAttackEndTimer(1.25f);
	}
	else if (AttackCombo == 5)
	{
		EKPlayer->StopAnimMontage(SpearAttackAnim);
		EKPlayer->PlayAnimMontage(SpearAttackAnim, 1.0f, FName("Attack5"));
		EKPlayerController->SetAttackEndTimer(1.33f);
	}

	EKPlayerController->ConsumtionStaminaAndTimer(SpearAttackStamina);
}

void ASpear::PlaySkillStartAnimMontage(AEKPlayer* EKPlayer, AEKPlayerController* EKPlayerController)
{
	if (!EKPlayerController->bIsEquipWeapon || !SpearSkillAnim)
	{
		return;
	}

	if (EKPlayer->GetPlayerStatusComponent()->GetStamina() < SpearSkill ||
		EKPlayer->GetPlayerStatusComponent()->GetMp() < SpearSkillMp)
	{
		EKPlayer->EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Attack);
		return;
	}

	EKPlayer->StopAnimMontage(SpearSkillAnim);
	EKPlayer->PlayAnimMontage(SpearSkillAnim);
	EKPlayer->GetPlayerStatusComponent()->SetMp(-SpearSkillMp);
	EKPlayerController->ConsumtionStaminaAndTimer(SpearSkill);
	EKPlayerController->RemoveAttackTagTimer(SpearSkillAnim->GetPlayLength());
}

#pragma endregion

void ASpear::AttachToDefenseSocket(AEKPlayerWeapon* Weapon, AEKPlayer* EKPlayer)
{
	if (Weapon)
	{
		USkeletalMeshComponent* MeshComp = EKPlayer->GetMesh();
		if (MeshComp)
		{
			Weapon->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("spear_defense_socket"));
		}
	}
}