// Made by Somalia Pirate

#include "EKPlayer.h"
#include "EKPlayerStatusComponent.h"
#include "EKPlayerController.h"
#include "Components/InventoryComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "../Weapon/EKPlayerWeapon.h"
#include "Player/Weapon/Staff/Staff.h"
#include "Player/Weapon/Spear/Spear.h"
#include "Player/Weapon/GreatSword/GreatSword.h"
#include "Animation/AnimInstance.h"
#include "../EKPlayerGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../Enemy/EK_EnemyBase.h"
#include "Components/BoxComponent.h"
#include "Player/Weapon/DamageType/EKPlayerDamageType.h"
#include "Engine/DamageEvents.h"
#include "Enemy/DamageSystem/EKDamageType.h"
#include "EKGameplayTags.h"
#include "UI/UISubsystem.h"
#include "UI/YouDied/Widget_YouDied.h"
#include "Subsystems/SanctuarySubsystem.h"
#include "Map/Sanctuary/EKSanctuary.h"
#include "Kismet/GameplayStatics.h"
#include "EKGameplayTags.h"
#include "Subsystems/SanctuarySubsystem.h"

AEKPlayer::AEKPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	AEKPlayer::GetCapsuleComponent()->InitCapsuleSize(34.f, 95.f);

	PlayerStatusComponent = CreateDefaultSubobject<UEKPlayerStatusComponent>(TEXT("PlayerStatusComponent"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshFinder(TEXT("/AssetShare/Character/DARK_C_KNIGHT/MESHES/UE5/SK_DC_Knight_UE5_full_Silver"));
	AEKPlayer::GetMesh()->SetSkeletalMesh(SkeletalMeshFinder.Object);
	AEKPlayer::GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -100), FRotator(0, -90, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;

	LeftLegCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftLeg"));
	LeftLegCapsuleComponent->SetupAttachment(RootComponent);
	LeftLegCapsuleComponent->SetRelativeLocation(FVector(20, -30, -70));

	GetCharacterMovement()->GravityScale = 3.f;
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	TargetFindLockOnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetFindLockOnBox"));
	TargetFindLockOnBox->SetupAttachment(RootComponent);
	TargetFindLockOnBox->SetRelativeLocation(FVector(0, 0, 0));
	TargetFindLockOnBox->SetRelativeScale3D(FVector(24.f, 24.f, 20.f));
}

void AEKPlayer::BeginPlay()
{
	Super::BeginPlay();

	EKPlayerController = Cast<AEKPlayerController>(this->GetController());

	TargetFindLockOnBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTargetEnterRange);
	TargetFindLockOnBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnTargetExitRange);

#pragma region Weapon Test

	// Test Change Weapon

	// Test GreatSword Version

	/*if (GreatSwordTypeAClass)
	{
		FActorSpawnParameters SpawnParams;
		CurrentWeapon = GetWorld()->SpawnActor<AEKPlayerWeapon>(GreatSwordTypeAClass, SpawnParams);
		AttachWeaponToSpineSocket(CurrentWeapon);
		EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_Equip_GreatSword);
	}*/

	/*if (GreatSwordTypeBClass)
	{
		FActorSpawnParameters SpawnParams;
		CurrentWeapon = GetWorld()->SpawnActor<AEKPlayerWeapon>(GreatSwordTypeBClass, SpawnParams);
		AttachWeaponToSpineSocket(CurrentWeapon);
		EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_Equip_GreatSword);
	}*/

	// Test Spear Version

	/*if (SpearTypeAClass)
	{
		FActorSpawnParameters SpawnParams;
		CurrentWeapon = GetWorld()->SpawnActor<AEKPlayerWeapon>(SpearTypeAClass, SpawnParams);
		AttachWeaponToSpineSocket(CurrentWeapon);
		EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_Equip_Spear);
	}*/

	/*if (SpearTypeBClass)
	{
		FActorSpawnParameters SpawnParams;
		CurrentWeapon = GetWorld()->SpawnActor<AEKPlayerWeapon>(SpearTypeBClass, SpawnParams);
		AttachWeaponToSpineSocket(CurrentWeapon);
		EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_Equip_Spear);
	}*/

	// Test Staff Version Don't Select This

	/*if (StaffTypeAClass)
	{
		FActorSpawnParameters SpawnParams;
		CurrentWeapon = GetWorld()->SpawnActor<AEKPlayerWeapon>(StaffTypeAClass, SpawnParams);
		AttachWeaponToSpineSocket(CurrentWeapon);
		EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_Equip_Staff);
	}*/

	/*if (StaffTypeBClass)
	{
		FActorSpawnParameters SpawnParams;
		CurrentWeapon = GetWorld()->SpawnActor<AEKPlayerWeapon>(StaffTypeBClass, SpawnParams);
		AttachWeaponToSpineSocket(CurrentWeapon);
		EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_Equip_Staff);
	}*/

#pragma endregion

}

void AEKPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LockOnTarget)
	{
		FVector Direction = LockOnTarget->GetActorLocation() - GetActorLocation();
		Direction = Direction.GetSafeNormal();

		FRotator TargetRotation = Direction.Rotation();

		TargetRotation.Pitch = EKPlayerController->GetControlRotation().Pitch;

		LockOnTargetRotation = TargetRotation;
		EKPlayerController->SetControlRotation(LockOnTargetRotation);
	}

	if (!CurrentWeapon)
	{
		return;
	}

	if (CurrentWeapon->CheckAnimMontageEnd(this, CurrentWeapon->GetWeaponAttackAnim()))
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

#pragma region Damage

float AEKPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (!EKPlayerController || 
		EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_State_Hit) ||
		EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_State_StrongHit) ||
		EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_State_Invincibility))
	{
		return 0.f;
	}

	if (CheckPlayerDie())
	{
		return 0.f;
	}

	UEKDamageType* DamageType = Cast<UEKDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());

	if (DamageType->IsA(UEKNormalDamageType::StaticClass()))
	{
		HitTimer();
	}
	else if (DamageType->IsA(UEKStrongDamageType::StaticClass()))
	{
		StrongHitTimer();
	}

	// if player don't equip weapon, equip weapon automatically
	if (!EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_State_BattleState))
	{
		if (!EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_GreatSword) &&
			!EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_Spear) &&
			!EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_Equip_Staff))
		{
			goto NextFunc;
		}

		CurrentWeapon->PlayWeaponEquipAnimMontage(this, EKPlayerController);
	}

NextFunc:

	EKPlayerController->BattleStateTimer();

	HitDirection(DamageCauser);

	// About Defense Logic
	if (EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_State_Defense) && PlayerStatusComponent->GetStamina() >= DefenseStamina && HitAngle >= -30 && HitAngle <= 30)
	{
		EKPlayerController->ConsumtionStaminaAndTimer(DefenseStamina);

		if (!EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_State_Invincibility))
		{
			PlayerStatusComponent->SetHp(-Damage * 0.3);
		}
	}
	else if (EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_State_DomainExpansion_HPPig))
	{
		if (!EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_State_Invincibility))
		{
			EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Defense);
			PlayerStatusComponent->SetHp(-Damage * 0.2);
		}
	}
	else // About Normal Logic
	{
		if (!EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_State_Invincibility))
		{
			EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Defense);
			PlayerStatusComponent->SetHp(-Damage);
		}
	}

	EKPlayerController->InvincibilityTimer(0.4f);

	return 0.f;
}

bool AEKPlayer::CheckPlayerDie()
{
	if (PlayerStatusComponent->GetHp() <= 0)
	{
		if (!EKPlayerStateContainer.HasTag(EKPlayerGameplayTags::EKPlayer_State_Die))
		{
			EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_State_Die);

			UUISubsystem* UISystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
			if (!UISystem) return true;
			UISystem->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_Game_Died, ESlateVisibility::SelfHitTestInvisible);

			UUserWidget* widget = UISystem->GetWidget(FEKGameplayTags::Get().UI_Widget_Game_Died);
			if (!widget) return true;
			UWidget_YouDied* userWidget = Cast<UWidget_YouDied>(widget);
			if (!userWidget) return true;

			userWidget->PlayerDied();

			GetWorld()->GetTimerManager().SetTimer(Handle_PlayerDied, this, &AEKPlayer::PlayerRestart,
				4, false);

			OnPlayerDieDelegate.Broadcast();
		}
		return true;
	}
	return false;
}

void AEKPlayer::HitDirection(AActor* Enemy)
{
	FVector PlayerLocation = this->GetActorLocation();
	FVector EnemyLocation = Enemy->GetActorLocation();

	FVector Direction = (EnemyLocation - PlayerLocation).GetSafeNormal();
	FVector PlayerForward = this->GetActorForwardVector();

	float DotProduct = FVector::DotProduct(Direction, PlayerForward);
	float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

	FVector CrossProduct = FVector::CrossProduct(PlayerForward, Direction);

	if (CrossProduct.Z < 0) {
		Angle = -Angle;
	}

	HitAngle = Angle;
}

#pragma endregion

#pragma region Attach to Socket

void AEKPlayer::EquipWeapon(const FWeaponStruct& InWeaponInfo)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	if (InWeaponInfo.WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		CurrentWeapon = GetWorld()->SpawnActor<AEKPlayerWeapon>(InWeaponInfo.WeaponClass, SpawnParams);
		if (!CurrentWeapon || !EKPlayerController) return;

		EKPlayerController->bIsEquipWeapon = false;
		AttachWeaponToSpineSocket(CurrentWeapon);

		EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_BattleState);
		EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_Equip_GreatSword);
		EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_Equip_Spear);
		EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_Equip_Staff);

		if (InWeaponInfo.WeaponClass.Get()->IsChildOf(AGreatSword::StaticClass()))
			EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_Equip_GreatSword);
		else if (InWeaponInfo.WeaponClass.Get()->IsChildOf(ASpear::StaticClass()))
			EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_Equip_Spear);
		else if (InWeaponInfo.WeaponClass.Get()->IsChildOf(AStaff::StaticClass()))
			EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_Equip_Staff);

		// GetMesh()->SetAnimInstanceClass(InWeaponInfo.AnimInstance);
	}
}

void AEKPlayer::UnEquipWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;

		EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_Equip_GreatSword);
		EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_Equip_Spear);
		EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_Equip_Staff);
		EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_BattleState);
	}
}

void AEKPlayer::AttachWeaponToSpineSocket(TObjectPtr<AEKPlayerWeapon> Weapon)
{
	if (Weapon)
	{
		Weapon->AttachWeaponToSpineSocket(Weapon, this);
	}
}

void AEKPlayer::AttachWeaponToHandSocket(TObjectPtr<AEKPlayerWeapon> Weapon)
{
	if (Weapon)
	{
		Weapon->AttachWeaponToHandSocket(Weapon, this);
	}
}

#pragma endregion

#pragma region Timer

void AEKPlayer::RemoveHitTag()
{
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Hit);
}

void AEKPlayer::HitTimer()
{
	EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_State_Hit);
	GetWorldTimerManager().SetTimer(HitTagHandle, this, &ThisClass::RemoveHitTag, NextHitTime, false);
}

void AEKPlayer::RemoveStrongHitTag()
{
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_StrongHit);
}

void AEKPlayer::StrongHitTimer()
{
	EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_State_StrongHit);
	GetWorldTimerManager().SetTimer(StrongHitTagHandle, this, &ThisClass::RemoveStrongHitTag, NextStrongHitTime, false);
}

void AEKPlayer::PlayerRestart()
{
	USanctuarySubsystem* SanctuarySystem = GetGameInstance()->GetSubsystem<USanctuarySubsystem>();
	if (!SanctuarySystem) return;
	AEKSanctuary* sanctuary = SanctuarySystem->GetSanctuary(this, SanctuarySystem->GetLastVisitIdx());
	if (!sanctuary) return;
	FVector STLocation = sanctuary->GetActorLocation();

	FHitResult hitResult;
	bool hit = GetWorld()->LineTraceSingleByChannel(hitResult,
		STLocation + FVector(0, 0, 150), STLocation - FVector(0, 0, 150), ECollisionChannel::ECC_Visibility);
	if (hit)
	{
		SetActorLocation(hitResult.Location);
	}
	else
	{
		SetActorLocation(STLocation + FVector(0, 0, 150));
	}

	// Widget Visibility Change
	UUISubsystem* UISystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
	if (!UISystem) return;
	UISystem->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_Game_BossBattle, ESlateVisibility::Collapsed);

	// Restore Map & State
	PlayerRestore();
}

void AEKPlayer::PlayerRestore()
{
	USanctuarySubsystem* SanctuarySystem = GetGameInstance()->GetSubsystem<USanctuarySubsystem>();
	if (!SanctuarySystem) return;

	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Die);

	StopAnimMontage();

	// Restore Player State & Restore Potion
	if (PlayerStatusComponent)
		PlayerStatusComponent->RestoreState();

	if (EKPlayerController && EKPlayerController->GetInventoryComponent())
	{
		EKPlayerController->GetInventoryComponent()->RestorePotionQuantity();
	}

	// Restore Map
	SanctuarySystem->RestoreAllMap(this);
}

#pragma endregion

#pragma region Lock On

void AEKPlayer::SetLockOnTarget(AActor* Target)
{
	LockOnTarget = Target;
}

void AEKPlayer::OnTargetEnterRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AEK_EnemyBase::StaticClass()))
	{
		LockOnTargets.Add(OtherActor);
	}
}

void AEKPlayer::OnTargetExitRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && LockOnTargets.Contains(OtherActor))
	{
		LockOnTargets.Remove(OtherActor);
	}
}

AActor* AEKPlayer::FindNearTarget()
{
	if (LockOnTargets.Num() == 0)
	{
		return nullptr;
	}

	float NearDistance = 10000.f;
	AActor* NearTarget = nullptr;

	for (AActor* Target : LockOnTargets)
	{
		if (IsValid(Target))
		{
			float Distance = FVector::Dist(this->GetActorLocation(), Target->GetActorLocation());

			if (Distance < NearDistance)
			{
				NearDistance = Distance;
				NearTarget = Target;
			}
		}
	}

	return NearTarget;
}

#pragma endregion

void AEKPlayer::PlayerRevive()
{
	if (!CheckPlayerDie())
	{
		return;
	}

	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_Equip_GreatSword);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_Equip_Spear);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_Equip_Staff);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Attack);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_BattleState);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Defense);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Die);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Dodge);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_DomainExpansion);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_DomainExpansion_HPPig);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_DomainExpansion_TimeSlow);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Hit);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Invincibility);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Jump);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Look);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Move);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_SitDown);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_Sprint);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_StrongHit);
	EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_UseItem);

	PlayerStatusComponent->SetStatusHandle();
}