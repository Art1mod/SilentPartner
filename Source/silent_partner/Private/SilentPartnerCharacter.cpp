// Fill out your copyright notice in the Description page of Project Settings.


#include "SilentPartnerCharacter.h"
#include "silent_partner/Public/DialogueManagerComponent.h"

ASilentPartnerCharacter::ASilentPartnerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the Dialogue Manager Component
	DialogueManager = CreateDefaultSubobject<UDialogueManagerComponent>(TEXT("DialogueManager"));
}

void ASilentPartnerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

UDialogueManagerComponent* ASilentPartnerCharacter::GetDialogueManager_Implementation()
{
	return DialogueManager;
}



