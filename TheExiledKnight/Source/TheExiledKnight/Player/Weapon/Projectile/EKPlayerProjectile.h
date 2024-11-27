// Made by Somalia Pirate

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Enemy/EK_EnemyBase.h"
#include "Player/Weapon/DamageType/EKPlayerDamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Player/EKPlayer/EKPlayer.h"
#include "Player/EKPlayer/EKPlayerController.h"
#include "Player/EKPlayer/EKPlayerStatusComponent.h"
#include "Player/Weapon/EKPlayerWeapon.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "EKPlayerProjectile.generated.h"

UCLASS()
class THEEXILEDKNIGHT_API AEKPlayerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AEKPlayerProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UCapsuleComponent* GetCollisionComponent() { return CapsuleComponent; }

	void SetBaseParticle(UParticleSystem* ChangeParticle);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	UParticleSystemComponent* BaseParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	USoundBase* HitSound;

protected:
	// Edit Editer Notify Damege Coefficient Value
	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageValue = 1.0;

protected:
	AEKPlayer* EKPlayer;
	AEKPlayerController* EKPlayerController;
};