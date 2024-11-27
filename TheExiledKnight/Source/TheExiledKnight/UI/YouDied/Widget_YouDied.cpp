// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/YouDied/Widget_YouDied.h"

void UWidget_YouDied::PlayerDied()
{
	PlayAnimation(Animation_Died);
}
