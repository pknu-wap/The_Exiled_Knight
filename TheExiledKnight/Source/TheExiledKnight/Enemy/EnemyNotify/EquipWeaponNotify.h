// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EquipWeaponNotify.generated.h"

/**
 * 
 */
UCLASS()
class THEEXILEDKNIGHT_API UEquipWeaponNotify : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket", meta = (AllowPrivateAccess = "true"))
	FName SocketName;


	
};
