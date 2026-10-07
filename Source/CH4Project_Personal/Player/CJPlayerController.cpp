// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CJPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/CJGameModeBase.h"
#include "UI/CJChatInput.h"
#include "EngineUtils.h"
#include "CH4Project_Personal.h"

void ACJPlayerController::BeginPlay()
{

	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

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

	//PrintChatMessageString(ChatMessageString);
	if (IsLocalController() == true)
	{
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void ACJPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	//UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Red, 5.0f);

	//FString NetModeString = Chap4FunctionLibrary::GetNetModeString(this);
	//FString CombinedMessageString = FString::Printf(TEXT("%s : %s"), *NetModeString, *InChatMessageString);
	//Chap4FunctionLibrary::MyPrintString(this, CombinedMessageString, 10.f);

	Chap4FunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void ACJPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ACJPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ACJGameModeBase* CJGM = Cast<ACJGameModeBase>(GM);
		if (IsValid(CJGM) == true)
		{
			CJGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}
