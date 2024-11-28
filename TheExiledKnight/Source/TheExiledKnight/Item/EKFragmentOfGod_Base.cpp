// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EKFragmentOfGod_Base.h"
#include "Subsystems/InventorySubsystem.h"
#include "Components/SphereComponent.h"

AEKFragmentOfGod_Base::AEKFragmentOfGod_Base()
{
}

void AEKFragmentOfGod_Base::BeginPlay()
{
	FFragmentOfGod* FOGInfo = GetWorld()->GetGameInstance<UInventorySubsystem>()->GetFOGInfo(ID);

	CoolDown = FOGInfo->CoolDown;
	Radius = FOGInfo->Radius;
	Duration = FOGInfo->Duration;

	Collision->SetSphereRadius(Radius);
	SetLifeSpan(Duration);
	/*
	�� �ٿ��� ���Ⱑ �ƴ϶� �÷��̾� ��Ʈ�ѷ����� Ű�� ���� �� ����Ǿ�� ��
	��, 
	*/
}

void AEKFragmentOfGod_Base::Tick(float DeltaTime)
{
}

void AEKFragmentOfGod_Base::UseItem(UWorld* WorldContext, float level)
{
}

void AEKFragmentOfGod_Base::Activate()
{
	// ���� ó�� ���� Ŀ���� �ڵ� �߰�
}
