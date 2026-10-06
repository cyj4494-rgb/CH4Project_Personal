// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CJPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/CJChatInput.h"

void ACJPlayerController::BeginPlay()
{

	Super::BeginPlay();

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true) {
		ChatInputWidgetInstance =  CreateWidget<UCJChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true){
			ChatInputWidgetInstance->AddToViewport();
		}
	}
}

void ACJPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	PrintChatMessageString(ChatMessageString);
}

void ACJPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Red, 5.0f);
}

