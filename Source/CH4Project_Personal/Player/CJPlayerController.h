// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CJPlayerController.generated.h"


class UCJChatInput;
/**
 * 
 */
UCLASS()
class CH4PROJECT_PERSONAL_API ACJPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(client , Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(server , Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCJChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UCJChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;
};
