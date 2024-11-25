// Made by Somalia Pirate

#include "WeaponSkillAttack.h"

void UWeaponSkillAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	EKPlayer = Cast<AEKPlayer>(MeshComp->GetOwner());
	if (EKPlayer)
	{
		EKPlayerController = Cast<AEKPlayerController>(EKPlayer->GetController());
	}
}

void UWeaponSkillAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!EKPlayer)
	{
		return;
	}

	UCapsuleComponent* WeaponCC = Cast<UCapsuleComponent>(EKPlayer->GetCurrentWeapon()->GetWeaponCapsuleComponent());

	if (!WeaponCC)
	{
		return;
	}

	FVector CapsuleLocation = WeaponCC->GetComponentLocation();
	FRotator CapsuleRotation = WeaponCC->GetComponentRotation();
	float CapsuleHalfHeight = WeaponCC->GetScaledCapsuleHalfHeight();
	float CapsuleRadius = WeaponCC->GetScaledCapsuleRadius();

	TArray<FHitResult> HitResults;

	bool bIsHit = MeshComp->GetWorld()->SweepMultiByChannel(
		HitResults,
		CapsuleLocation,
		CapsuleLocation,
		CapsuleRotation.Quaternion(),
		ECC_Pawn,
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight)
	);

	if (!bIsHit)
	{
		return;
	}

	for (auto& Hit : HitResults)
	{
		AEK_EnemyBase* HitEnemy = Cast<AEK_EnemyBase>(Hit.GetActor());

		if (IgnoreEnemy.Contains(HitEnemy))
		{
			continue;
		}

		if (HitEnemy)
		{
			IgnoreEnemy.Emplace(HitEnemy);
			TSubclassOf<UEKPlayerStrongDamageType> PlayerDamageType = UEKPlayerStrongDamageType::StaticClass();
			UGameplayStatics::ApplyDamage(HitEnemy, EKPlayer->GetPlayerStatusComponent()->GetPlayerFinalDamage() * DamageValue, EKPlayerController, EKPlayer->GetCurrentWeapon(), PlayerDamageType);
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Silver, TEXT("Skill"));
		}
	}
}

void UWeaponSkillAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	IgnoreEnemy.Empty();
}