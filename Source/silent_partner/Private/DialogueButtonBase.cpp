// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueButtonBase.h"

void UDialogueButtonBase::NotifyClicked()
{
	OnDialogueButtonClicked.Broadcast(ChoiceID);
}
