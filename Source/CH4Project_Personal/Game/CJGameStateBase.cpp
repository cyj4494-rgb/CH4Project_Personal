// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CJGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CJPlayerController.h"

void ACJGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false) {
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ACJPlayerController* CJPC = Cast<ACJPlayerController>(PC);
			if (IsValid(CJPC) == true) {
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CJPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
