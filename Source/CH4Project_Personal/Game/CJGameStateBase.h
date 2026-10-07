// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CJGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CH4PROJECT_PERSONAL_API ACJGameStateBase : public AGameStateBase
{
	GENERATED_BODY()	

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
	
};
