// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyNotify/EquipWeaponNotify.h"
#include"Enemy/EK_EnemyBase.h"

void UEquipWeaponNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation); 

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)return;

	AEK_EnemyBase *OwnerEnemy = Cast<AEK_EnemyBase>(OwnerActor);
	if (!OwnerEnemy||!OwnerEnemy->Weapon)return; 

    OwnerEnemy->Weapon->AttachToComponent
	(
        MeshComp,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        SocketName
    );
   


}
