// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CJPawn.h"
#include "CH4Project_Personal.h"

// Called when the game starts or when spawned
void ACJPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = Chap4FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CJPawn::BeginPlay() %s [%s]"), *Chap4FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Chap4FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
	
}

void ACJPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = Chap4FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CJPawn::PossessedBy() %s [%s]"), *Chap4FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Chap4FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}


