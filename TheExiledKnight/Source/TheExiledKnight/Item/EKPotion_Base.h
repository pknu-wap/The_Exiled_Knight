// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/EKItem_Base.h"
#include "EKPotion_Base.generated.h"

/**
 * 
 */
UCLASS()
class THEEXILEDKNIGHT_API AEKPotion_Base : public AEKItem_Base
{
	GENERATED_BODY()
	
public:
	AEKPotion_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void UseItem(UWorld* WorldContext, int level = 1) override;
};
